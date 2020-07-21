/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/
#include "juce_AudioFormatFileHelpers.h"

namespace juce
{


//==============================================================================
class WavAudioFormatReader  : public AudioFormatReader
{
public:
    WavAudioFormatReader (InputStream* in)  : AudioFormatReader (in, wavFormatName)
    {
        doRead (in, false);
    }

    virtual void readWithChunkStorage (bool thumbnailOnly_) override
    {
        thumbnailOnly = thumbnailOnly_;
        input->setPosition (0);
        doRead (input, true);
    }

    void doRead (InputStream* in, bool storeChunks)
    {
        using namespace WavFileHelpers;
        uint64 len = 0, end = 0;
        int cueNoteIndex = 0;
        int cueLabelIndex = 0;
        int cueRegionIndex = 0;
        
        auto streamStartPos = input->getPosition();
        auto firstChunkType = input->readInt();
        
        if (firstChunkType == chunkName ("RF64"))
        {
            input->skipNextBytes (4); // size is -1 for RF64
            isRF64 = true;
        }
        else if (firstChunkType == chunkName ("RIFF"))
        {
            len = (uint64) (uint32) input->readInt();
            end = len + (uint64) input->getPosition();
        }
        else
        {
            return;
        }
        
        auto startOfRIFFChunk = input->getPosition();
        
        if (input->readInt() == chunkName ("WAVE"))
        {
            if (isRF64 && input->readInt() == chunkName ("ds64"))
            {
                auto length = (uint32) input->readInt();
                
                if (length < 28)
                    return;
                
                auto chunkEnd = input->getPosition() + length + (length & 1);
                len = (uint64) input->readInt64();
                end = len + (uint64) startOfRIFFChunk;
                dataLength = input->readInt64();
                input->setPosition (chunkEnd);
            }
            
            while ((uint64) input->getPosition() < end && ! input->isExhausted())
            {
                auto chunkType = input->readInt();
                auto length = (uint32) input->readInt();
                auto chunkEnd = input->getPosition() + length + (length & 1);
                
                if (!thumbnailOnly && storeChunks)
                {
                    // BR: MOD vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
                    MetadataChunk* chunk = chunkCollection.getOrCreateChunkWithName (chunkType);
                    // Copy the chunk into a memory block
                    int64 chunkStart = input->getPosition();
                    input->readIntoMemoryBlock (chunk->data, (ssize_t) length);
                    input->setPosition (chunkStart);
                    // end br: mod
                }

                if (chunkType == chunkName ("fmt "))
                {
                    // read the format chunk
                    auto format = (unsigned short) input->readShort();
                    numChannels = (unsigned int) input->readShort();
                    sampleRate = input->readInt();
                    auto bytesPerSec = input->readInt();
                    input->skipNextBytes (2);
                    bitsPerSample = (unsigned int) (int) input->readShort();
                    
                    if (bitsPerSample > 64)
                    {
                        bytesPerFrame = bytesPerSec / (int) sampleRate;
                        bitsPerSample = 8 * (unsigned int) bytesPerFrame / numChannels;
                    }
                    else
                    {
                        bytesPerFrame = numChannels * bitsPerSample / 8;
                    }
                    
                    if (format == 3)
                    {
                        usesFloatingPointData = true;
                    }
                    else if (format == 0xfffe) // WAVE_FORMAT_EXTENSIBLE
                    {
                        if (length < 40) // too short
                        {
                            bytesPerFrame = 0;
                        }
                        else
                        {
                            input->skipNextBytes (4); // skip over size and bitsPerSample
                            auto channelMask = input->readInt();
                            metadataValues.set ("ChannelMask", String (channelMask));
                            channelLayout = getChannelLayoutFromMask (channelMask, numChannels);
                            
                            ExtensibleWavSubFormat subFormat;
                            subFormat.data1 = (uint32) input->readInt();
                            subFormat.data2 = (uint16) input->readShort();
                            subFormat.data3 = (uint16) input->readShort();
                            input->read (subFormat.data4, sizeof (subFormat.data4));
                            
                            if (subFormat == IEEEFloatFormat)
                                usesFloatingPointData = true;
                            else if (subFormat != pcmFormat && subFormat != ambisonicFormat)
                                bytesPerFrame = 0;
                        }
                    }
                    else if (format == 0x674f  // WAVE_FORMAT_OGG_VORBIS_MODE_1
                             || format == 0x6750  // WAVE_FORMAT_OGG_VORBIS_MODE_2
                             || format == 0x6751  // WAVE_FORMAT_OGG_VORBIS_MODE_3
                             || format == 0x676f  // WAVE_FORMAT_OGG_VORBIS_MODE_1_PLUS
                             || format == 0x6770  // WAVE_FORMAT_OGG_VORBIS_MODE_2_PLUS
                             || format == 0x6771) // WAVE_FORMAT_OGG_VORBIS_MODE_3_PLUS
                    {
                        isSubformatOggVorbis = true;
                        sampleRate = 0; // to mark the wav reader as failed
                        input->setPosition (streamStartPos);
                        return;
                    }
                    else if (format != 1)
                    {
                        bytesPerFrame = 0;
                    }
                }
                else if (chunkType == chunkName ("data"))
                {
                    if (! isRF64) // data size is expected to be -1, actual data size is in ds64 chunk
                        dataLength = length;
                    
                    dataChunkStart = input->getPosition();
                    lengthInSamples = (bytesPerFrame > 0) ? (dataLength / bytesPerFrame) : 0;
                }
                else if (chunkType == chunkName ("ID3 "))
                {
                    if (thumbnailOnly || storeChunks)
                    {
                        input->readIntoMemoryBlock (id3Data, (ssize_t) length);
                    }
                }
                else if (chunkType == chunkName ("iXML"))
                {
//#if SCAN_EXPERIMENT
//#else
                    MemoryBlock ii;
                    input->readIntoMemoryBlock (ii, (ssize_t) length);
                    if (ii.getData())
                    {
                        String s = String::createStringFromData (ii.getData(), ii.getSize());
                        //                        Logger::outputDebugString (s);
                        metadataValues.set ("iXML", s);
                    }
//#endif
                }
                
                else if (chunkType == chunkName ("bext"))
                {
                    bwavChunkStart = input->getPosition();
                    bwavSize = length;
                    
                    HeapBlock<BWAVChunk> bwav;
                    bwav.calloc (jmax ((size_t) length + 1, sizeof (BWAVChunk)), 1);
                    input->read (bwav, (int) length);
                    bwav->copyTo (metadataValues, (int) length);
                }
                else if (chunkType == chunkName ("smpl"))
                {
                    HeapBlock<SMPLChunk> smpl;
                    smpl.calloc (jmax ((size_t) length + 1, sizeof (SMPLChunk)), 1);
                    input->read (smpl, (int) length);
                    smpl->copyTo (metadataValues, (int) length);
                }
                else if (chunkType == chunkName ("inst") || chunkType == chunkName ("INST")) // need to check which...
                {
                    HeapBlock<InstChunk> inst;
                    inst.calloc (jmax ((size_t) length + 1, sizeof (InstChunk)), 1);
                    input->read (inst, (int) length);
                    inst->copyTo (metadataValues);
                }
                else if (chunkType == chunkName ("cue "))
                {
                    HeapBlock<CueChunk> cue;
                    cue.calloc (jmax ((size_t) length + 1, sizeof (CueChunk)), 1);
                    input->read (cue, (int) length);
                    cue->copyTo (metadataValues, (int) length);
                }
                else if (chunkType == chunkName ("axml"))
                {
                    MemoryBlock axml;
                    input->readIntoMemoryBlock (axml, (ssize_t) length);
                    AXMLChunk::addToMetadata (metadataValues, axml.toString());
                }
                else if (chunkType == chunkName ("LIST"))
                {
                    auto subChunkType = input->readInt();
                    
                    if (subChunkType == chunkName ("info") || subChunkType == chunkName ("INFO"))
                    {
                        ListInfoChunk::addToMetadata (metadataValues, *input, chunkEnd);
                    }
                    else if (subChunkType == chunkName ("adtl"))
                    {
                        while (input->getPosition() < chunkEnd)
                        {
                            auto adtlChunkType = input->readInt();
                            auto adtlLength = (uint32) input->readInt();
                            auto adtlChunkEnd = input->getPosition() + (adtlLength + (adtlLength & 1));
                            
                            if (adtlChunkType == chunkName ("labl") || adtlChunkType == chunkName ("note"))
                            {
                                String prefix;
                                
                                if (adtlChunkType == chunkName ("labl"))
                                    prefix << "CueLabel" << cueLabelIndex++;
                                else if (adtlChunkType == chunkName ("note"))
                                    prefix << "CueNote" << cueNoteIndex++;
                                
                                auto identifier = (uint32) input->readInt();
                                auto stringLength = (int) adtlLength - 4;
                                
                                MemoryBlock textBlock;
                                input->readIntoMemoryBlock (textBlock, stringLength);
                                
                                metadataValues.set (prefix + "Identifier", String (identifier));
                                metadataValues.set (prefix + "Text", textBlock.toString());
                            }
                            else if (adtlChunkType == chunkName ("ltxt"))
                            {
                                auto prefix = "CueRegion" + String (cueRegionIndex++);
                                auto identifier     = (uint32) input->readInt();
                                auto sampleLength   = (uint32) input->readInt();
                                auto purpose        = (uint32) input->readInt();
                                auto country        = (uint16) input->readShort();
                                auto language       = (uint16) input->readShort();
                                auto dialect        = (uint16) input->readShort();
                                auto codePage       = (uint16) input->readShort();
                                auto stringLength   = adtlLength - 20;
                                
                                MemoryBlock textBlock;
                                input->readIntoMemoryBlock (textBlock, (int) stringLength);
                                
                                metadataValues.set (prefix + "Identifier",   String (identifier));
                                metadataValues.set (prefix + "SampleLength", String (sampleLength));
                                metadataValues.set (prefix + "Purpose",      String (purpose));
                                metadataValues.set (prefix + "Country",      String (country));
                                metadataValues.set (prefix + "Language",     String (language));
                                metadataValues.set (prefix + "Dialect",      String (dialect));
                                metadataValues.set (prefix + "CodePage",     String (codePage));
                                metadataValues.set (prefix + "Text",         textBlock.toString());
                            }
                            
                            input->setPosition (adtlChunkEnd);
                        }
                    }
                }
                else if (chunkType == chunkName ("acid"))
                {
                    AcidChunk (*input, length).addToMetadata (metadataValues);
                }
                else if (chunkType == chunkName ("Trkn"))
                {
                    MemoryBlock tracktion;
                    input->readIntoMemoryBlock (tracktion, (ssize_t) length);
                    metadataValues.set (WavAudioFormat::tracktionLoopInfo, tracktion.toString());
                }
                else if (chunkEnd <= input->getPosition())
                {
                    break;
                }
                
                input->setPosition (chunkEnd);
            }
        }
        
        if (cueLabelIndex > 0)          metadataValues.set ("NumCueLabels",    String (cueLabelIndex));
        if (cueNoteIndex > 0)           metadataValues.set ("NumCueNotes",     String (cueNoteIndex));
        if (cueRegionIndex > 0)         metadataValues.set ("NumCueRegions",   String (cueRegionIndex));
        if (metadataValues.size() > 0)  metadataValues.set ("MetaDataSource",  "WAV");

    }
    //==============================================================================
    bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
                      int64 startSampleInFile, int numSamples) override
    {
        clearSamplesBeyondAvailableLength (destSamples, numDestChannels, startOffsetInDestBuffer,
                                           startSampleInFile, numSamples, lengthInSamples);

        if (numSamples <= 0)
            return true;

        input->setPosition (dataChunkStart + startSampleInFile * bytesPerFrame);

        while (numSamples > 0)
        {
            const int tempBufSize = 480 * 3 * 4; // (keep this a multiple of 3)
            char tempBuffer[tempBufSize];

            auto numThisTime = jmin (tempBufSize / bytesPerFrame, numSamples);
            auto bytesRead = input->read (tempBuffer, numThisTime * bytesPerFrame);

            if (bytesRead < numThisTime * bytesPerFrame)
            {
                jassert (bytesRead >= 0);
                zeromem (tempBuffer + bytesRead, (size_t) (numThisTime * bytesPerFrame - bytesRead));
            }

            copySampleData (bitsPerSample, usesFloatingPointData,
                            destSamples, startOffsetInDestBuffer, numDestChannels,
                            tempBuffer, (int) numChannels, numThisTime);

            startOffsetInDestBuffer += numThisTime;
            numSamples -= numThisTime;
        }

        return true;
    }

    static void copySampleData (unsigned int bitsPerSample, const bool usesFloatingPointData,
                                int* const* destSamples, int startOffsetInDestBuffer, int numDestChannels,
                                const void* sourceData, int numChannels, int numSamples) noexcept
    {
        switch (bitsPerSample)
        {
            case 8:     ReadHelper<AudioData::Int32, AudioData::UInt8, AudioData::LittleEndian>::read (destSamples, startOffsetInDestBuffer, numDestChannels, sourceData, numChannels, numSamples); break;
            case 16:    ReadHelper<AudioData::Int32, AudioData::Int16, AudioData::LittleEndian>::read (destSamples, startOffsetInDestBuffer, numDestChannels, sourceData, numChannels, numSamples); break;
            case 24:    ReadHelper<AudioData::Int32, AudioData::Int24, AudioData::LittleEndian>::read (destSamples, startOffsetInDestBuffer, numDestChannels, sourceData, numChannels, numSamples); break;
            case 32:    if (usesFloatingPointData) ReadHelper<AudioData::Float32, AudioData::Float32, AudioData::LittleEndian>::read (destSamples, startOffsetInDestBuffer, numDestChannels, sourceData, numChannels, numSamples);
                        else                       ReadHelper<AudioData::Int32,   AudioData::Int32,   AudioData::LittleEndian>::read (destSamples, startOffsetInDestBuffer, numDestChannels, sourceData, numChannels, numSamples);
                        break;
            default:    jassertfalse; break;
        }
    }

    //==============================================================================
    AudioChannelSet getChannelLayout() override
    {
        if (channelLayout.size() == static_cast<int> (numChannels))
            return channelLayout;

        return WavFileHelpers::canonicalWavChannelSet (static_cast<int> (numChannels));
    }

    static AudioChannelSet getChannelLayoutFromMask (int dwChannelMask, size_t totalNumChannels)
    {
        AudioChannelSet wavFileChannelLayout;

        // AudioChannelSet and wav's dwChannelMask are compatible
        BigInteger channelBits (dwChannelMask);

        for (auto bit = channelBits.findNextSetBit (0); bit >= 0; bit = channelBits.findNextSetBit (bit + 1))
            wavFileChannelLayout.addChannel (static_cast<AudioChannelSet::ChannelType> (bit + 1));

        // channel layout and number of channels do not match
        if (wavFileChannelLayout.size() != static_cast<int> (totalNumChannels))
        {
            // for backward compatibility with old wav files, assume 1 or 2
            // channel wav files are mono/stereo respectively
            if (totalNumChannels <= 2 && dwChannelMask == 0)
                wavFileChannelLayout = AudioChannelSet::canonicalChannelSet (static_cast<int> (totalNumChannels));
            else
            {
                auto discreteSpeaker = static_cast<int> (AudioChannelSet::discreteChannel0);

                while (wavFileChannelLayout.size() < static_cast<int> (totalNumChannels))
                    wavFileChannelLayout.addChannel (static_cast<AudioChannelSet::ChannelType> (discreteSpeaker++));
            }
        }

        return wavFileChannelLayout;
    }

    int64 bwavChunkStart = 0, bwavSize = 0;
    int64 dataChunkStart = 0, dataLength = 0;
    int bytesPerFrame = 0;
    bool isRF64 = false;
    bool isSubformatOggVorbis = false;

    AudioChannelSet channelLayout;

private:
    
    bool thumbnailOnly = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavAudioFormatReader)
};

//==============================================================================
class WavAudioFormatWriter  : public AudioFormatWriter
{
public:
    WavAudioFormatWriter (OutputStream* const out, const double rate,
                          const AudioChannelSet& channelLayoutToUse, const unsigned int bits,
                          const StringPairArray& metadataValues, AudioFormatReader::ChunkCollection* chunkCollection_ = nullptr)
        : AudioFormatWriter (out, wavFormatName, rate, channelLayoutToUse, bits),
        chunkCollection(chunkCollection_)
    {
        using namespace WavFileHelpers;

        if (metadataValues.size() > 0)
        {
            // The meta data should have been sanitised for the WAV format.
            // If it was originally sourced from an AIFF file the MetaDataSource
            // key should be removed (or set to "WAV") once this has been done
            jassert (metadataValues.getValue ("MetaDataSource", "None") != "AIFF");

            bwavChunk     = BWAVChunk::createFrom (metadataValues);
            axmlChunk     = AXMLChunk::createFrom (metadataValues);
            smplChunk     = SMPLChunk::createFrom (metadataValues);
            instChunk     = InstChunk::createFrom (metadataValues);
            cueChunk      = CueChunk ::createFrom (metadataValues);
            listChunk     = ListChunk::createFrom (metadataValues);
            listInfoChunk = ListInfoChunk::createFrom (metadataValues);
            acidChunk     = AcidChunk::createFrom (metadataValues);
            trckChunk     = TracktionChunk::createFrom (metadataValues);
        }

        headerPosition = out->getPosition();
        writeHeader (chunkCollection);
    }

    ~WavAudioFormatWriter() override
    {
        writeHeader();
    }

    //==============================================================================
    bool write (const int** data, int numSamples) override
    {
        jassert (numSamples >= 0);
        jassert (data != nullptr && *data != nullptr); // the input must contain at least one channel!

        if (writeFailed)
            return false;

        auto bytes = numChannels * (size_t) numSamples * bitsPerSample / 8;
        tempBlock.ensureSize (bytes, false);

        switch (bitsPerSample)
        {
            case 8:     WriteHelper<AudioData::UInt8, AudioData::Int32, AudioData::LittleEndian>::write (tempBlock.getData(), (int) numChannels, data, numSamples); break;
            case 16:    WriteHelper<AudioData::Int16, AudioData::Int32, AudioData::LittleEndian>::write (tempBlock.getData(), (int) numChannels, data, numSamples); break;
            case 24:    WriteHelper<AudioData::Int24, AudioData::Int32, AudioData::LittleEndian>::write (tempBlock.getData(), (int) numChannels, data, numSamples); break;
            case 32:    WriteHelper<AudioData::Int32, AudioData::Int32, AudioData::LittleEndian>::write (tempBlock.getData(), (int) numChannels, data, numSamples); break;
            default:    jassertfalse; break;
        }

        if (! output->write (tempBlock.getData(), bytes))
        {
            // failed to write to disk, so let's try writing the header.
            // If it's just run out of disk space, then if it does manage
            // to write the header, we'll still have a useable file..
            writeHeader();
            writeFailed = true;
            return false;
        }

        bytesWritten += bytes;
        lengthInSamples += (uint64) numSamples;
        return true;
    }

    bool flush() override
    {
        auto lastWritePos = output->getPosition();
        writeHeader();

        if (output->setPosition (lastWritePos))
            return true;

        // if this fails, you've given it an output stream that can't seek! It needs
        // to be able to seek back to write the header
        jassertfalse;
        return false;
    }

private:
    MemoryBlock tempBlock, bwavChunk, axmlChunk, smplChunk, instChunk, cueChunk, listChunk, listInfoChunk, acidChunk, trckChunk;
    uint64 lengthInSamples = 0, bytesWritten = 0;
    int64 headerPosition = 0;
    bool writeFailed = false;

    // br: mod line below
    AudioFormatReader::ChunkCollection* chunkCollection;
    
    void writeHeader (AudioFormatReader::ChunkCollection* chunkCollection_ = nullptr)
    {
        
        if (chunkCollection_)
        {
            chunkCollection = chunkCollection_;
        }
        if ((bytesWritten & 1) != 0) // pad to an even length
            output->writeByte (0);

        using namespace WavFileHelpers;

        if (headerPosition != output->getPosition() && ! output->setPosition (headerPosition))
        {
            // if this fails, you've given it an output stream that can't seek! It needs to be
            // able to seek back to go back and write the header after the data has been written.
            jassertfalse;
            return;
        }

        const size_t bytesPerFrame = numChannels * bitsPerSample / 8;
        uint64 audioDataSize = bytesPerFrame * lengthInSamples;
        auto channelMask = getChannelMaskFromChannelLayout (channelLayout);

        const bool isRF64 = (bytesWritten >= 0x100000000LL);
        const bool isWaveFmtEx = isRF64 || (channelMask != 0);

        
        // br: mod
#if 1
        int64 riffChunkSize = (int64) (4 /* 'RIFF' */ + 8 + 40 /* WAVEFORMATEX */
                                       + 8 + audioDataSize + (audioDataSize & 1));

        if (chunkCollection)
        {
            for (int i = 0; i < chunkCollection->storedChunks.size(); ++i)
            {
                AudioFormatReader::MetadataChunk* chunk = chunkCollection->storedChunks[i].get();
                if (!chunk->isMetadataChunk())
                {
                    continue;
                }
                
                if (chunk->data.getSize() & 1)
                {
                    char z = 0;
                    chunk->data.append (&z, 1);
                }
                riffChunkSize += chunkSize (chunk->data);
            }

        }
        
        riffChunkSize += 8 + 28; // (ds64 chunk)

                                       /*
                                       
                                       
                                       + chunkSize (bwavChunk)
                                       + chunkSize (axmlChunk)
                                       + chunkSize (smplChunk)
                                       + chunkSize (instChunk)
                                       + chunkSize (cueChunk)
                                       + chunkSize (listChunk)
                                       + chunkSize (listInfoChunk)
                                       + chunkSize (acidChunk)
                                       + chunkSize (trckChunk)
                                       + (8 + 28)); // (ds64 chunk)
        */
        riffChunkSize += (riffChunkSize & 1);

#else
        int64 riffChunkSize = (int64) (4 /* 'RIFF' */ + 8 + 40 /* WAVEFORMATEX */
                                       + 8 + audioDataSize + (audioDataSize & 1)
                                       + chunkSize (bwavChunk)
                                       + chunkSize (axmlChunk)
                                       + chunkSize (smplChunk)
                                       + chunkSize (instChunk)
                                       + chunkSize (cueChunk)
                                       + chunkSize (listChunk)
                                       + chunkSize (listInfoChunk)
                                       + chunkSize (acidChunk)
                                       + chunkSize (trckChunk)
                                       + (8 + 28)); // (ds64 chunk)

        riffChunkSize += (riffChunkSize & 1);
#endif
        if (isRF64)
            writeChunkHeader (chunkName ("RF64"), -1);
        else
            writeChunkHeader (chunkName ("RIFF"), (int) riffChunkSize);

        output->writeInt (chunkName ("WAVE"));

        if (! isRF64)
        {
           #if ! JUCE_WAV_DO_NOT_PAD_HEADER_SIZE
            /* NB: This junk chunk is added for padding, so that the header is a fixed size
               regardless of whether it's RF64 or not. That way, we can begin recording a file,
               and when it's finished, can go back and write either a RIFF or RF64 header,
               depending on whether more than 2^32 samples were written.

               The JUCE_WAV_DO_NOT_PAD_HEADER_SIZE macro allows you to disable this feature in case
               you need to create files for crappy WAV players with bugs that stop them skipping chunks
               which they don't recognise. But DO NOT USE THIS option unless you really have no choice,
               because it means that if you write more than 2^32 samples to the file, you'll corrupt it.
            */
            writeChunkHeader (chunkName ("JUNK"), 28 + (isWaveFmtEx? 0 : 24));
            output->writeRepeatedByte (0, 28 /* ds64 */ + (isWaveFmtEx? 0 : 24));
           #endif
        }
        else
        {
           #if JUCE_WAV_DO_NOT_PAD_HEADER_SIZE
            // If you disable padding, then you MUST NOT write more than 2^32 samples to a file.
            jassertfalse;
           #endif

            writeChunkHeader (chunkName ("ds64"), 28);  // chunk size for uncompressed data (no table)
            output->writeInt64 (riffChunkSize);
            output->writeInt64 ((int64) audioDataSize);
            output->writeRepeatedByte (0, 12);
        }

        if (isWaveFmtEx)
        {
            writeChunkHeader (chunkName ("fmt "), 40);
            output->writeShort ((short) (uint16) 0xfffe); // WAVE_FORMAT_EXTENSIBLE
        }
        else
        {
            writeChunkHeader (chunkName ("fmt "), 16);
            output->writeShort (bitsPerSample < 32 ? (short) 1 /*WAVE_FORMAT_PCM*/
                                                   : (short) 3 /*WAVE_FORMAT_IEEE_FLOAT*/);
        }

        output->writeShort ((short) numChannels);
        output->writeInt ((int) sampleRate);
        output->writeInt ((int) (bytesPerFrame * sampleRate)); // nAvgBytesPerSec
        output->writeShort ((short) bytesPerFrame); // nBlockAlign
        output->writeShort ((short) bitsPerSample); // wBitsPerSample

        if (isWaveFmtEx)
        {
            output->writeShort (22); // cbSize (size of the extension)
            output->writeShort ((short) bitsPerSample); // wValidBitsPerSample
            output->writeInt (channelMask);

            const ExtensibleWavSubFormat& subFormat = bitsPerSample < 32 ? pcmFormat : IEEEFloatFormat;

            output->writeInt ((int) subFormat.data1);
            output->writeShort ((short) subFormat.data2);
            output->writeShort ((short) subFormat.data3);
            output->write (subFormat.data4, sizeof (subFormat.data4));
        }

        // br: mod
        if (chunkCollection)
        {
            for (int i = 0; i < chunkCollection->storedChunks.size(); ++i)
            {
                AudioFormatReader::MetadataChunk* chunk = chunkCollection->storedChunks[i].get();
                if (!chunk->isMetadataChunk())
                {
                    continue;
                }
                
                int chName = chunk->name;
                int chSize = chunk->data.getSize();
                
                if (chunk->name == chunkName ("inst"))
                {
                    writeChunk (chunk->data, chunk->name, 7);
                }
                else
                {
                    writeChunk (chunk->data, chunk->name);
                }
            }
        }
        /*
        writeChunk (bwavChunk,     chunkName ("bext"));
        writeChunk (axmlChunk,     chunkName ("axml"));
        writeChunk (smplChunk,     chunkName ("smpl"));
        writeChunk (instChunk,     chunkName ("inst"), 7);
        writeChunk (cueChunk,      chunkName ("cue "));
        writeChunk (listChunk,     chunkName ("LIST"));
        writeChunk (listInfoChunk, chunkName ("LIST"));
        writeChunk (acidChunk,     chunkName ("acid"));
        writeChunk (trckChunk,     chunkName ("Trkn"));
         */
        
        
        writeChunkHeader (chunkName ("data"), isRF64 ? -1 : (int) (lengthInSamples * bytesPerFrame));

        usesFloatingPointData = (bitsPerSample == 32);
    }

    static size_t chunkSize (const MemoryBlock& data) noexcept     { return data.getSize() > 0 ? (8 + data.getSize()) : 0; }

    void writeChunkHeader (int chunkType, int size) const
    {
        output->writeInt (chunkType);
        output->writeInt (size);
    }

    void writeChunk (const MemoryBlock& data, int chunkType, int size = 0) const
    {
        if (data.getSize() > 0)
        {
            writeChunkHeader (chunkType, size != 0 ? size : (int) data.getSize());
            *output << data;
        }
    }

    static int getChannelMaskFromChannelLayout (const AudioChannelSet& channelLayout)
    {
        if (channelLayout.isDiscreteLayout())
            return 0;

        // Don't add an extended format chunk for mono and stereo. Basically, all wav players
        // interpret a wav file with only one or two channels to be mono or stereo anyway.
        if (channelLayout == AudioChannelSet::mono() || channelLayout == AudioChannelSet::stereo())
            return 0;

        auto channels = channelLayout.getChannelTypes();
        auto wavChannelMask = 0;

        for (auto channel : channels)
        {
            int wavChannelBit = static_cast<int> (channel) - 1;
            jassert (wavChannelBit >= 0 && wavChannelBit <= 31);

            wavChannelMask |= (1 << wavChannelBit);
        }

        return wavChannelMask;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavAudioFormatWriter)
};

//==============================================================================
class MemoryMappedWavReader   : public MemoryMappedAudioFormatReader
{
public:
    MemoryMappedWavReader (const File& wavFile, const WavAudioFormatReader& reader)
        : MemoryMappedAudioFormatReader (wavFile, reader, reader.dataChunkStart,
                                         reader.dataLength, reader.bytesPerFrame)
    {
    }

    bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
                      int64 startSampleInFile, int numSamples) override
    {
        clearSamplesBeyondAvailableLength (destSamples, numDestChannels, startOffsetInDestBuffer,
                                           startSampleInFile, numSamples, lengthInSamples);

        if (map == nullptr || ! mappedSection.contains (Range<int64> (startSampleInFile, startSampleInFile + numSamples)))
        {
            jassertfalse; // you must make sure that the window contains all the samples you're going to attempt to read.
            return false;
        }

        WavAudioFormatReader::copySampleData (bitsPerSample, usesFloatingPointData,
                                              destSamples, startOffsetInDestBuffer, numDestChannels,
                                              sampleToPointer (startSampleInFile), (int) numChannels, numSamples);
        return true;
    }

    void getSample (int64 sample, float* result) const noexcept override
    {
        auto num = (int) numChannels;

        if (map == nullptr || ! mappedSection.contains (sample))
        {
            jassertfalse; // you must make sure that the window contains all the samples you're going to attempt to read.

            zeromem (result, sizeof (float) * (size_t) num);
            return;
        }

        auto dest = &result;
        auto source = sampleToPointer (sample);

        switch (bitsPerSample)
        {
            case 8:     ReadHelper<AudioData::Float32, AudioData::UInt8, AudioData::LittleEndian>::read (dest, 0, 1, source, 1, num); break;
            case 16:    ReadHelper<AudioData::Float32, AudioData::Int16, AudioData::LittleEndian>::read (dest, 0, 1, source, 1, num); break;
            case 24:    ReadHelper<AudioData::Float32, AudioData::Int24, AudioData::LittleEndian>::read (dest, 0, 1, source, 1, num); break;
            case 32:    if (usesFloatingPointData) ReadHelper<AudioData::Float32, AudioData::Float32, AudioData::LittleEndian>::read (dest, 0, 1, source, 1, num);
                        else                       ReadHelper<AudioData::Float32, AudioData::Int32,   AudioData::LittleEndian>::read (dest, 0, 1, source, 1, num);
                        break;
            default:    jassertfalse; break;
        }
    }

    void readMaxLevels (int64 startSampleInFile, int64 numSamples, Range<float>* results, int numChannelsToRead) override
    {
        numSamples = jmin (numSamples, lengthInSamples - startSampleInFile);

        if (map == nullptr || numSamples <= 0 || ! mappedSection.contains (Range<int64> (startSampleInFile, startSampleInFile + numSamples)))
        {
            jassert (numSamples <= 0); // you must make sure that the window contains all the samples you're going to attempt to read.

            for (int i = 0; i < numChannelsToRead; ++i)
                results[i] = {};

            return;
        }

        switch (bitsPerSample)
        {
            case 8:     scanMinAndMax<AudioData::UInt8> (startSampleInFile, numSamples, results, numChannelsToRead); break;
            case 16:    scanMinAndMax<AudioData::Int16> (startSampleInFile, numSamples, results, numChannelsToRead); break;
            case 24:    scanMinAndMax<AudioData::Int24> (startSampleInFile, numSamples, results, numChannelsToRead); break;
            case 32:    if (usesFloatingPointData) scanMinAndMax<AudioData::Float32> (startSampleInFile, numSamples, results, numChannelsToRead);
                        else                       scanMinAndMax<AudioData::Int32>   (startSampleInFile, numSamples, results, numChannelsToRead);
                        break;
            default:    jassertfalse; break;
        }
    }

private:
    template <typename SampleType>
    void scanMinAndMax (int64 startSampleInFile, int64 numSamples, Range<float>* results, int numChannelsToRead) const noexcept
    {
        for (int i = 0; i < numChannelsToRead; ++i)
            results[i] = scanMinAndMaxInterleaved<SampleType, AudioData::LittleEndian> (i, startSampleInFile, numSamples);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MemoryMappedWavReader)
};

//==============================================================================
WavAudioFormat::WavAudioFormat()  : AudioFormat (wavFormatName, ".wav .bwf") {}
WavAudioFormat::~WavAudioFormat() {}

Array<int> WavAudioFormat::getPossibleSampleRates()
{
    return { 8000,  11025, 12000, 16000,  22050,  32000,  44100,
             48000, 88200, 96000, 176400, 192000, 352800, 384000 };
}

Array<int> WavAudioFormat::getPossibleBitDepths()
{
    return { 8, 16, 24, 32 };
}

bool WavAudioFormat::canDoStereo()  { return true; }
bool WavAudioFormat::canDoMono()    { return true; }

bool WavAudioFormat::isChannelLayoutSupported (const AudioChannelSet& channelSet)
{
    auto channelTypes = channelSet.getChannelTypes();

    // When
    if (channelSet.isDiscreteLayout())
        return true;

    // WAV supports all channel types from left ... topRearRight
    for (auto channel : channelTypes)
        if (channel < AudioChannelSet::left || channel > AudioChannelSet::topRearRight)
            return false;

    return true;
}

AudioFormatReader* WavAudioFormat::createReaderFor (InputStream* sourceStream, bool deleteStreamIfOpeningFails)
{
    std::unique_ptr<WavAudioFormatReader> r (new WavAudioFormatReader (sourceStream));

   #if JUCE_USE_OGGVORBIS
    if (r->isSubformatOggVorbis)
    {
        r->input = nullptr;
        return OggVorbisAudioFormat().createReaderFor (sourceStream, deleteStreamIfOpeningFails);
    }
   #endif

    if (r->sampleRate > 0 && r->numChannels > 0 && r->bytesPerFrame > 0 && r->bitsPerSample <= 32)
        return r.release();

    if (! deleteStreamIfOpeningFails)
        r->input = nullptr;

    return nullptr;
}

MemoryMappedAudioFormatReader* WavAudioFormat::createMemoryMappedReader (const File& file)
{
    return createMemoryMappedReader (file.createInputStream());
}

MemoryMappedAudioFormatReader* WavAudioFormat::createMemoryMappedReader (FileInputStream* fin)
{
    if (fin != nullptr)
    {
        WavAudioFormatReader reader (fin);

        if (reader.lengthInSamples > 0)
            return new MemoryMappedWavReader (fin->getFile(), reader);
    }

    return nullptr;
}

AudioFormatWriter* WavAudioFormat::createWriterFor (OutputStream* out, double sampleRate,
                                                    unsigned int numChannels, int bitsPerSample,
                                                    const StringPairArray& metadataValues, int qualityOptionIndex)
{
    return createWriterFor (out, sampleRate, WavFileHelpers::canonicalWavChannelSet (static_cast<int> (numChannels)),
                            bitsPerSample, metadataValues, qualityOptionIndex);
}

AudioFormatWriter* WavAudioFormat::createWriterFor (OutputStream* out,
                                                    double sampleRate,
                                                    const AudioChannelSet& channelLayout,
                                                    int bitsPerSample,
                                                    const StringPairArray& metadataValues,
                                                    int /*qualityOptionIndex*/)
{
    if (out != nullptr && getPossibleBitDepths().contains (bitsPerSample) && isChannelLayoutSupported (channelLayout))
        return new WavAudioFormatWriter (out, sampleRate, channelLayout,
                                         (unsigned int) bitsPerSample, metadataValues);

    return nullptr;
}

    // br: mod added this function
AudioFormatWriter* WavAudioFormat::createWriterFor (OutputStream* out,
                                                    double sampleRate,
                                                    const AudioChannelSet& channelLayout,
                                                    int bitsPerSample,
                                                    const StringPairArray& metadataValues,
                                                    int /*qualityOptionIndex*/, AudioFormatReader::ChunkCollection* chunkCollection)
{
    if (out != nullptr && getPossibleBitDepths().contains (bitsPerSample) && isChannelLayoutSupported (channelLayout))
        return new WavAudioFormatWriter (out, sampleRate, channelLayout,
                                         (unsigned int) bitsPerSample, metadataValues, chunkCollection);
    
    return nullptr;
}


namespace WavFileHelpers
{
    static bool slowCopyWavFileWithNewMetadata (const File& file, const StringPairArray& metadata, AudioFormatReader::ChunkCollection* chunkCollection = nullptr)
    {
        TemporaryFile tempFile (file);
        WavAudioFormat wav;

        std::unique_ptr<AudioFormatReader> reader (wav.createReaderFor (file.createInputStream(), true));

        if (reader != nullptr)
        {
            std::unique_ptr<OutputStream> outStream (tempFile.getFile().createOutputStream());

            if (outStream != nullptr)
            {
                
                // br: mod vvvvvvvvvvvvvvv

                std::unique_ptr<AudioFormatWriter> writer (wav.createWriterFor (outStream.get(), reader->sampleRate,
                                                                                WavFileHelpers::canonicalWavChannelSet (static_cast<int> (reader->numChannels)), (int) reader->bitsPerSample,
                                                                                metadata, 0, chunkCollection));

                /*
                std::unique_ptr<AudioFormatWriter> writer (wav.createWriterFor (outStream.get(), reader->sampleRate,
                                                                                reader->numChannels, (int) reader->bitsPerSample,
                                                                                metadata, 0, chunkCollection));
                */
                if (writer != nullptr)
                {
                    outStream.release();
                    
                    bool ok = writer->writeFromAudioReader (*reader, 0, -1);
                    writer.reset();
                    reader.reset();

                    return ok && tempFile.overwriteTargetFileWithTemporary();
                }
            }
        }

        
        return false;
    }
}

    
bool WavAudioFormat::replaceMetadataInFile (const File& wavFile, const StringPairArray& newMetadata, AudioFormatReader::ChunkCollection* chunkCollection)
{

    using namespace WavFileHelpers;

    // br: mod
    /*

    std::unique_ptr<WavAudioFormatReader> reader (static_cast<WavAudioFormatReader*> (createReaderFor (wavFile.createInputStream(), true)));

    if (reader != nullptr)
    {
        auto bwavPos  = reader->bwavChunkStart;
        auto bwavSize = reader->bwavSize;
        reader.reset();

        if (bwavSize > 0)
        {
            auto chunk = BWAVChunk::createFrom (newMetadata);

            if (chunk.getSize() <= (size_t) bwavSize)
            {
                // the new one will fit in the space available, so write it directly..
                auto oldSize = wavFile.getSize();

                {
                    FileOutputStream out (wavFile);

                    if (out.openedOk())
                    {
                        out.setPosition (bwavPos);
                        out << chunk;
                        out.setPosition (oldSize);
                    }
                }

                jassert (wavFile.getSize() == oldSize);
                return true;
            }
        }
    }
*/
    return slowCopyWavFileWithNewMetadata (wavFile, newMetadata, chunkCollection);
}

//==============================================================================
#if JUCE_UNIT_TESTS

struct WaveAudioFormatTests : public UnitTest
{
    WaveAudioFormatTests() : UnitTest ("Wave audio format tests") {}

    void runTest() override
    {
        beginTest ("Setting up metadata");

        StringPairArray metadataValues = WavAudioFormat::createBWAVMetadata ("description",
                                                                             "originator",
                                                                             "originatorRef",
                                                                             Time::getCurrentTime(),
                                                                             numTestAudioBufferSamples,
                                                                             "codingHistory");

        for (int i = numElementsInArray (WavFileHelpers::ListInfoChunk::types); --i >= 0;)
            metadataValues.set (WavFileHelpers::ListInfoChunk::types[i],
                                WavFileHelpers::ListInfoChunk::types[i]);

        if (metadataValues.size() > 0)
            metadataValues.set ("MetaDataSource", "WAV");

        metadataValues.addArray (createDefaultSMPLMetadata());

        WavAudioFormat format;
        MemoryBlock memoryBlock;

        {
            beginTest ("Creating a basic wave writer");

            std::unique_ptr<AudioFormatWriter> writer (format.createWriterFor (new MemoryOutputStream (memoryBlock, false),
                                                                               44100.0, numTestAudioBufferChannels,
                                                                               32, metadataValues, 0));
            expect (writer != nullptr);

            AudioBuffer<float> buffer (numTestAudioBufferChannels, numTestAudioBufferSamples);
            buffer.clear();

            beginTest ("Writing audio data to the basic wave writer");
            expect (writer->writeFromAudioSampleBuffer (buffer, 0, numTestAudioBufferSamples));
        }

        {
            beginTest ("Creating a basic wave reader");

            std::unique_ptr<AudioFormatReader> reader (format.createReaderFor (new MemoryInputStream (memoryBlock, false), false));
            expect (reader != nullptr);
            expect (reader->metadataValues == metadataValues, "Somehow, the metadata is different!");
        }
    }

private:
    enum
    {
        numTestAudioBufferChannels = 2,
        numTestAudioBufferSamples = 256
    };

    StringPairArray createDefaultSMPLMetadata() const
    {
        StringPairArray m;

        m.set ("Manufacturer", "0");
        m.set ("Product", "0");
        m.set ("SamplePeriod", "0");
        m.set ("MidiUnityNote", "60");
        m.set ("MidiPitchFraction", "0");
        m.set ("SmpteFormat", "0");
        m.set ("SmpteOffset", "0");
        m.set ("NumSampleLoops", "0");
        m.set ("SamplerData", "0");

        return m;
    }

    JUCE_DECLARE_NON_COPYABLE (WaveAudioFormatTests)
};

static const WaveAudioFormatTests waveAudioFormatTests;

#endif

} // namespace juce

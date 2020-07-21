#pragma once

namespace juce
{
    

static const char* const wavFormatName = "WAV file";

//==============================================================================
const char* const WavAudioFormat::bwavDescription      = "bwav description";
const char* const WavAudioFormat::bwavOriginator       = "bwav originator";
const char* const WavAudioFormat::bwavOriginatorRef    = "bwav originator ref";
const char* const WavAudioFormat::bwavOriginationDate  = "bwav origination date";
const char* const WavAudioFormat::bwavOriginationTime  = "bwav origination time";
const char* const WavAudioFormat::bwavTimeReference    = "bwav time reference";
const char* const WavAudioFormat::bwavCodingHistory    = "bwav coding history";

StringPairArray WavAudioFormat::createBWAVMetadata (const String& description,
                                                    const String& originator,
                                                    const String& originatorRef,
                                                    Time date,
                                                    int64 timeReferenceSamples,
                                                    const String& codingHistory)
{
    StringPairArray m;
    
    m.set (bwavDescription, description);
    m.set (bwavOriginator, originator);
    m.set (bwavOriginatorRef, originatorRef);
    m.set (bwavOriginationDate, date.formatted ("%Y-%m-%d"));
    m.set (bwavOriginationTime, date.formatted ("%H:%M:%S"));
    m.set (bwavTimeReference, String (timeReferenceSamples));
    m.set (bwavCodingHistory, codingHistory);
    
    return m;
}

const char* const WavAudioFormat::acidOneShot          = "acid one shot";
const char* const WavAudioFormat::acidRootSet          = "acid root set";
const char* const WavAudioFormat::acidStretch          = "acid stretch";
const char* const WavAudioFormat::acidDiskBased        = "acid disk based";
const char* const WavAudioFormat::acidizerFlag         = "acidizer flag";
const char* const WavAudioFormat::acidRootNote         = "acid root note";
const char* const WavAudioFormat::acidBeats            = "acid beats";
const char* const WavAudioFormat::acidDenominator      = "acid denominator";
const char* const WavAudioFormat::acidNumerator        = "acid numerator";
const char* const WavAudioFormat::acidTempo            = "acid tempo";

const char* const WavAudioFormat::riffInfoArchivalLocation      = "IARL";
const char* const WavAudioFormat::riffInfoArtist                = "IART";
const char* const WavAudioFormat::riffInfoBaseURL               = "IBSU";
const char* const WavAudioFormat::riffInfoCinematographer       = "ICNM";
const char* const WavAudioFormat::riffInfoComment               = "CMNT";
const char* const WavAudioFormat::riffInfoComment2              = "ICMT";
const char* const WavAudioFormat::riffInfoComments              = "COMM";
const char* const WavAudioFormat::riffInfoCommissioned          = "ICMS";
const char* const WavAudioFormat::riffInfoCopyright             = "ICOP";
const char* const WavAudioFormat::riffInfoCostumeDesigner       = "ICDS";
const char* const WavAudioFormat::riffInfoCountry               = "ICNT";
const char* const WavAudioFormat::riffInfoCropped               = "ICRP";
const char* const WavAudioFormat::riffInfoDateCreated           = "ICRD";
const char* const WavAudioFormat::riffInfoDateTimeOriginal      = "IDIT";
const char* const WavAudioFormat::riffInfoDefaultAudioStream    = "ICAS";
const char* const WavAudioFormat::riffInfoDimension             = "IDIM";
const char* const WavAudioFormat::riffInfoDirectory             = "DIRC";
const char* const WavAudioFormat::riffInfoDistributedBy         = "IDST";
const char* const WavAudioFormat::riffInfoDotsPerInch           = "IDPI";
const char* const WavAudioFormat::riffInfoEditedBy              = "IEDT";
const char* const WavAudioFormat::riffInfoEighthLanguage        = "IAS8";
const char* const WavAudioFormat::riffInfoEncodedBy             = "CODE";
const char* const WavAudioFormat::riffInfoEndTimecode           = "TCDO";
const char* const WavAudioFormat::riffInfoEngineer              = "IENG";
const char* const WavAudioFormat::riffInfoFifthLanguage         = "IAS5";
const char* const WavAudioFormat::riffInfoFirstLanguage         = "IAS1";
const char* const WavAudioFormat::riffInfoFourthLanguage        = "IAS4";
const char* const WavAudioFormat::riffInfoGenre                 = "GENR";
const char* const WavAudioFormat::riffInfoKeywords              = "IKEY";
const char* const WavAudioFormat::riffInfoLanguage              = "LANG";
const char* const WavAudioFormat::riffInfoLength                = "TLEN";
const char* const WavAudioFormat::riffInfoLightness             = "ILGT";
const char* const WavAudioFormat::riffInfoLocation              = "LOCA";
const char* const WavAudioFormat::riffInfoLogoIconURL           = "ILIU";
const char* const WavAudioFormat::riffInfoLogoURL               = "ILGU";
const char* const WavAudioFormat::riffInfoMedium                = "IMED";
const char* const WavAudioFormat::riffInfoMoreInfoBannerImage   = "IMBI";
const char* const WavAudioFormat::riffInfoMoreInfoBannerURL     = "IMBU";
const char* const WavAudioFormat::riffInfoMoreInfoText          = "IMIT";
const char* const WavAudioFormat::riffInfoMoreInfoURL           = "IMIU";
const char* const WavAudioFormat::riffInfoMusicBy               = "IMUS";
const char* const WavAudioFormat::riffInfoNinthLanguage         = "IAS9";
const char* const WavAudioFormat::riffInfoNumberOfParts         = "PRT2";
const char* const WavAudioFormat::riffInfoOrganisation          = "TORG";
const char* const WavAudioFormat::riffInfoPart                  = "PRT1";
const char* const WavAudioFormat::riffInfoProducedBy            = "IPRO";
const char* const WavAudioFormat::riffInfoProductName           = "IPRD";
const char* const WavAudioFormat::riffInfoProductionDesigner    = "IPDS";
const char* const WavAudioFormat::riffInfoProductionStudio      = "ISDT";
const char* const WavAudioFormat::riffInfoRate                  = "RATE";
const char* const WavAudioFormat::riffInfoRated                 = "AGES";
const char* const WavAudioFormat::riffInfoRating                = "IRTD";
const char* const WavAudioFormat::riffInfoRippedBy              = "IRIP";
const char* const WavAudioFormat::riffInfoSecondaryGenre        = "ISGN";
const char* const WavAudioFormat::riffInfoSecondLanguage        = "IAS2";
const char* const WavAudioFormat::riffInfoSeventhLanguage       = "IAS7";
const char* const WavAudioFormat::riffInfoSharpness             = "ISHP";
const char* const WavAudioFormat::riffInfoSixthLanguage         = "IAS6";
const char* const WavAudioFormat::riffInfoSoftware              = "ISFT";
const char* const WavAudioFormat::riffInfoSoundSchemeTitle      = "DISP";
const char* const WavAudioFormat::riffInfoSource                = "ISRC";
const char* const WavAudioFormat::riffInfoSourceFrom            = "ISRF";
const char* const WavAudioFormat::riffInfoStarring_ISTR         = "ISTR";
const char* const WavAudioFormat::riffInfoStarring_STAR         = "STAR";
const char* const WavAudioFormat::riffInfoStartTimecode         = "TCOD";
const char* const WavAudioFormat::riffInfoStatistics            = "STAT";
const char* const WavAudioFormat::riffInfoSubject               = "ISBJ";
const char* const WavAudioFormat::riffInfoTapeName              = "TAPE";
const char* const WavAudioFormat::riffInfoTechnician            = "ITCH";
const char* const WavAudioFormat::riffInfoThirdLanguage         = "IAS3";
const char* const WavAudioFormat::riffInfoTimeCode              = "ISMP";
const char* const WavAudioFormat::riffInfoTitle                 = "INAM";
const char* const WavAudioFormat::riffInfoTrackNo               = "IPRT";
const char* const WavAudioFormat::riffInfoTrackNumber           = "TRCK";
const char* const WavAudioFormat::riffInfoURL                   = "TURL";
const char* const WavAudioFormat::riffInfoVegasVersionMajor     = "VMAJ";
const char* const WavAudioFormat::riffInfoVegasVersionMinor     = "VMIN";
const char* const WavAudioFormat::riffInfoVersion               = "TVER";
const char* const WavAudioFormat::riffInfoWatermarkURL          = "IWMU";
const char* const WavAudioFormat::riffInfoWrittenBy             = "IWRI";
const char* const WavAudioFormat::riffInfoYear                  = "YEAR";

const char* const WavAudioFormat::ISRC                 = "ISRC";
const char* const WavAudioFormat::tracktionLoopInfo    = "tracktion loop info";

//==============================================================================
namespace WavFileHelpers
{
    JUCE_CONSTEXPR inline int chunkName (const char* name) noexcept         { return (int) ByteOrder::littleEndianInt (name); }
    JUCE_CONSTEXPR inline size_t roundUpSize (size_t sz) noexcept           { return (sz + 3) & ~3u; }
    
#if JUCE_MSVC
#pragma pack (push, 1)
#endif
    
    struct iXMLChunk
    {
        String xmlString;
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            
            String ixml = values["iXML"];
            if (ixml.length() == 0)
            {
                MemoryBlock d (0);
                return d;
            }
            
            //            MemoryBlock data(ixml.toRawUTF8( ), ixml.getNumBytesAsUTF8( ) + 1);
            uint32 numBytes = ixml.getNumBytesAsUTF8();
            if (numBytes % 2)
            {
                numBytes++;
            }
            
            MemoryBlock data(numBytes, true);
            data.copyFrom (ixml.toRawUTF8(), 0, ixml.getNumBytesAsUTF8());
            return data;
            
        }
        
    } JUCE_PACKED;
    
    struct BWAVChunk
    {
        char description[256];
        char originator[32];
        char originatorRef[32];
        char originationDate[10];
        char originationTime[8];
        uint32 timeRefLow;
        uint32 timeRefHigh;
        uint16 version;
        uint8 umid[64];
        uint8 reserved[190];
        char codingHistory[1];
        
        void copyTo (StringPairArray& values, const int totalSize) const
        {
            values.set (WavAudioFormat::bwavDescription,     String::fromUTF8 (description,     sizeof (description)));
            values.set (WavAudioFormat::bwavOriginator,      String::fromUTF8 (originator,      sizeof (originator)));
            values.set (WavAudioFormat::bwavOriginatorRef,   String::fromUTF8 (originatorRef,   sizeof (originatorRef)));
            values.set (WavAudioFormat::bwavOriginationDate, String::fromUTF8 (originationDate, sizeof (originationDate)));
            values.set (WavAudioFormat::bwavOriginationTime, String::fromUTF8 (originationTime, sizeof (originationTime)));
            
            auto timeLow  = ByteOrder::swapIfBigEndian (timeRefLow);
            auto timeHigh = ByteOrder::swapIfBigEndian (timeRefHigh);
            auto time = (((int64) timeHigh) << 32) + timeLow;
            
            values.set (WavAudioFormat::bwavTimeReference, String (time));
            values.set (WavAudioFormat::bwavCodingHistory,
                        String::fromUTF8 (codingHistory, totalSize - (int) offsetof (BWAVChunk, codingHistory)));
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            MemoryBlock data (roundUpSize (sizeof (BWAVChunk) + values[WavAudioFormat::bwavCodingHistory].getNumBytesAsUTF8()));
            data.fillWith (0);
            
            auto* b = (BWAVChunk*) data.getData();
            
            // Allow these calls to overwrite an extra byte at the end, which is fine as long
            // as they get called in the right order..
            values[WavAudioFormat::bwavDescription]    .copyToUTF8 (b->description, 257);
            values[WavAudioFormat::bwavOriginator]     .copyToUTF8 (b->originator, 33);
            values[WavAudioFormat::bwavOriginatorRef]  .copyToUTF8 (b->originatorRef, 33);
            values[WavAudioFormat::bwavOriginationDate].copyToUTF8 (b->originationDate, 11);
            values[WavAudioFormat::bwavOriginationTime].copyToUTF8 (b->originationTime, 9);
            
            auto time = values[WavAudioFormat::bwavTimeReference].getLargeIntValue();
            b->timeRefLow = ByteOrder::swapIfBigEndian ((uint32) (time & 0xffffffff));
            b->timeRefHigh = ByteOrder::swapIfBigEndian ((uint32) (time >> 32));
            
            values[WavAudioFormat::bwavCodingHistory].copyToUTF8 (b->codingHistory, 0x7fffffff);
            
            if (b->description[0] != 0
                || b->originator[0] != 0
                || b->originationDate[0] != 0
                || b->originationTime[0] != 0
                || b->codingHistory[0] != 0
                || time != 0)
            {
                return data;
            }
            
            return {};
        }
        
    } JUCE_PACKED;
    
    //==============================================================================
    inline AudioChannelSet canonicalWavChannelSet (int numChannels)
    {
        if (numChannels == 1)  return AudioChannelSet::mono();
        if (numChannels == 2)  return AudioChannelSet::stereo();
        if (numChannels == 3)  return AudioChannelSet::createLCR();
        if (numChannels == 4)  return AudioChannelSet::quadraphonic();
        if (numChannels == 5)  return AudioChannelSet::create5point0();
        if (numChannels == 6)  return AudioChannelSet::create5point1();
        if (numChannels == 7)  return AudioChannelSet::create7point0SDDS();
        if (numChannels == 8)  return AudioChannelSet::create7point1SDDS();
        
        return AudioChannelSet::discreteChannels (numChannels);
    }
    
    //==============================================================================
    struct SMPLChunk
    {
        struct SampleLoop
        {
            uint32 identifier;
            uint32 type; // these are different in AIFF and WAV
            uint32 start;
            uint32 end;
            uint32 fraction;
            uint32 playCount;
        } JUCE_PACKED;
        
        uint32 manufacturer;
        uint32 product;
        uint32 samplePeriod;
        uint32 midiUnityNote;
        uint32 midiPitchFraction;
        uint32 smpteFormat;
        uint32 smpteOffset;
        uint32 numSampleLoops;
        uint32 samplerData;
        SampleLoop loops[1];
        
        template <typename NameType>
        static void setValue (StringPairArray& values, NameType name, uint32 val)
        {
            values.set (name, String (ByteOrder::swapIfBigEndian (val)));
        }
        
        static void setValue (StringPairArray& values, int prefix, const char* name, uint32 val)
        {
            setValue (values, "Loop" + String (prefix) + name, val);
        }
        
        void copyTo (StringPairArray& values, const int totalSize) const
        {
            setValue (values, "Manufacturer",      manufacturer);
            setValue (values, "Product",           product);
            setValue (values, "SamplePeriod",      samplePeriod);
            setValue (values, "MidiUnityNote",     midiUnityNote);
            setValue (values, "MidiPitchFraction", midiPitchFraction);
            setValue (values, "SmpteFormat",       smpteFormat);
            setValue (values, "SmpteOffset",       smpteOffset);
            setValue (values, "NumSampleLoops",    numSampleLoops);
            setValue (values, "SamplerData",       samplerData);
            
            for (int i = 0; i < (int) numSampleLoops; ++i)
            {
                if ((uint8*) (loops + (i + 1)) > ((uint8*) this) + totalSize)
                    break;
                
                setValue (values, i, "Identifier", loops[i].identifier);
                setValue (values, i, "Type",       loops[i].type);
                setValue (values, i, "Start",      loops[i].start);
                setValue (values, i, "End",        loops[i].end);
                setValue (values, i, "Fraction",   loops[i].fraction);
                setValue (values, i, "PlayCount",  loops[i].playCount);
            }
        }
        
        template <typename NameType>
        static uint32 getValue (const StringPairArray& values, NameType name, const char* def)
        {
            return ByteOrder::swapIfBigEndian ((uint32) values.getValue (name, def).getIntValue());
        }
        
        static uint32 getValue (const StringPairArray& values, int prefix, const char* name, const char* def)
        {
            return getValue (values, "Loop" + String (prefix) + name, def);
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            MemoryBlock data;
            auto numLoops = jmin (64, values.getValue ("NumSampleLoops", "0").getIntValue());
            
            data.setSize (roundUpSize (sizeof (SMPLChunk) + (size_t) (jmax (0, numLoops - 1)) * sizeof (SampleLoop)), true);
            
            auto s = static_cast<SMPLChunk*> (data.getData());
            
            s->manufacturer      = getValue (values, "Manufacturer", "0");
            s->product           = getValue (values, "Product", "0");
            s->samplePeriod      = getValue (values, "SamplePeriod", "0");
            s->midiUnityNote     = getValue (values, "MidiUnityNote", "60");
            s->midiPitchFraction = getValue (values, "MidiPitchFraction", "0");
            s->smpteFormat       = getValue (values, "SmpteFormat", "0");
            s->smpteOffset       = getValue (values, "SmpteOffset", "0");
            s->numSampleLoops    = ByteOrder::swapIfBigEndian ((uint32) numLoops);
            s->samplerData       = getValue (values, "SamplerData", "0");
            
            for (int i = 0; i < numLoops; ++i)
            {
                auto& loop = s->loops[i];
                loop.identifier = getValue (values, i, "Identifier", "0");
                loop.type       = getValue (values, i, "Type", "0");
                loop.start      = getValue (values, i, "Start", "0");
                loop.end        = getValue (values, i, "End", "0");
                loop.fraction   = getValue (values, i, "Fraction", "0");
                loop.playCount  = getValue (values, i, "PlayCount", "0");
            }
            
            return data;
        }
    } JUCE_PACKED;
    
    //==============================================================================
    struct InstChunk
    {
        int8 baseNote;
        int8 detune;
        int8 gain;
        int8 lowNote;
        int8 highNote;
        int8 lowVelocity;
        int8 highVelocity;
        
        static void setValue (StringPairArray& values, const char* name, int val)
        {
            values.set (name, String (val));
        }
        
        void copyTo (StringPairArray& values) const
        {
            setValue (values, "MidiUnityNote",  baseNote);
            setValue (values, "Detune",         detune);
            setValue (values, "Gain",           gain);
            setValue (values, "LowNote",        lowNote);
            setValue (values, "HighNote",       highNote);
            setValue (values, "LowVelocity",    lowVelocity);
            setValue (values, "HighVelocity",   highVelocity);
        }
        
        static int8 getValue (const StringPairArray& values, const char* name, const char* def)
        {
            return (int8) values.getValue (name, def).getIntValue();
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            MemoryBlock data;
            auto& keys = values.getAllKeys();
            
            if (keys.contains ("LowNote", true) && keys.contains ("HighNote", true))
            {
                data.setSize (8, true);
                auto* inst = static_cast<InstChunk*> (data.getData());
                
                inst->baseNote      = getValue (values, "MidiUnityNote", "60");
                inst->detune        = getValue (values, "Detune", "0");
                inst->gain          = getValue (values, "Gain", "0");
                inst->lowNote       = getValue (values, "LowNote", "0");
                inst->highNote      = getValue (values, "HighNote", "127");
                inst->lowVelocity   = getValue (values, "LowVelocity", "1");
                inst->highVelocity  = getValue (values, "HighVelocity", "127");
            }
            
            return data;
        }
    } JUCE_PACKED;
    
    //==============================================================================
    struct CueChunk
    {
        struct Cue
        {
            uint32 identifier;
            uint32 order;
            uint32 chunkID;
            uint32 chunkStart;
            uint32 blockStart;
            uint32 offset;
        } JUCE_PACKED;
        
        uint32 numCues;
        Cue cues[1];
        
        static void setValue (StringPairArray& values, int prefix, const char* name, uint32 val)
        {
            values.set ("Cue" + String (prefix) + name, String (ByteOrder::swapIfBigEndian (val)));
        }
        
        void copyTo (StringPairArray& values, const int totalSize) const
        {
            values.set ("NumCuePoints", String (ByteOrder::swapIfBigEndian (numCues)));
            
            for (int i = 0; i < (int) numCues; ++i)
            {
                if ((uint8*) (cues + (i + 1)) > ((uint8*) this) + totalSize)
                    break;
                
                setValue (values, i, "Identifier",  cues[i].identifier);
                setValue (values, i, "Order",       cues[i].order);
                setValue (values, i, "ChunkID",     cues[i].chunkID);
                setValue (values, i, "ChunkStart",  cues[i].chunkStart);
                setValue (values, i, "BlockStart",  cues[i].blockStart);
                setValue (values, i, "Offset",      cues[i].offset);
            }
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            MemoryBlock data;
            const int numCues = values.getValue ("NumCuePoints", "0").getIntValue();
            
            if (numCues > 0)
            {
                data.setSize (roundUpSize (sizeof (CueChunk) + (size_t) (numCues - 1) * sizeof (Cue)), true);
                
                auto c = static_cast<CueChunk*> (data.getData());
                
                c->numCues = ByteOrder::swapIfBigEndian ((uint32) numCues);
                
                const String dataChunkID (chunkName ("data"));
                int nextOrder = 0;
                
#if JUCE_DEBUG
                Array<uint32> identifiers;
#endif
                
                for (int i = 0; i < numCues; ++i)
                {
                    auto prefix = "Cue" + String (i);
                    auto identifier = (uint32) values.getValue (prefix + "Identifier", "0").getIntValue();
                    
#if JUCE_DEBUG
                    jassert (! identifiers.contains (identifier));
                    identifiers.add (identifier);
#endif
                    
                    auto order = values.getValue (prefix + "Order", String (nextOrder)).getIntValue();
                    nextOrder = jmax (nextOrder, order) + 1;
                    
                    auto& cue = c->cues[i];
                    cue.identifier   = ByteOrder::swapIfBigEndian ((uint32) identifier);
                    cue.order        = ByteOrder::swapIfBigEndian ((uint32) order);
                    cue.chunkID      = ByteOrder::swapIfBigEndian ((uint32) values.getValue (prefix + "ChunkID", dataChunkID).getIntValue());
                    cue.chunkStart   = ByteOrder::swapIfBigEndian ((uint32) values.getValue (prefix + "ChunkStart", "0").getIntValue());
                    cue.blockStart   = ByteOrder::swapIfBigEndian ((uint32) values.getValue (prefix + "BlockStart", "0").getIntValue());
                    cue.offset       = ByteOrder::swapIfBigEndian ((uint32) values.getValue (prefix + "Offset", "0").getIntValue());
                }
            }
            
            return data;
        }
        
    } JUCE_PACKED;
    
    //==============================================================================
    namespace ListChunk
    {
        static int getValue (const StringPairArray& values, const String& name)
        {
            return values.getValue (name, "0").getIntValue();
        }
        
        static int getValue (const StringPairArray& values, const String& prefix, const char* name)
        {
            return getValue (values, prefix + name);
        }
        
        static void appendLabelOrNoteChunk (const StringPairArray& values, const String& prefix,
                                            const int chunkType, MemoryOutputStream& out)
        {
            auto label = values.getValue (prefix + "Text", prefix);
            auto labelLength = (int) label.getNumBytesAsUTF8() + 1;
            auto chunkLength = 4 + labelLength + (labelLength & 1);
            
            out.writeInt (chunkType);
            out.writeInt (chunkLength);
            out.writeInt (getValue (values, prefix, "Identifier"));
            out.write (label.toUTF8(), (size_t) labelLength);
            
            if ((out.getDataSize() & 1) != 0)
                out.writeByte (0);
        }
        
        static void appendExtraChunk (const StringPairArray& values, const String& prefix, MemoryOutputStream& out)
        {
            auto text = values.getValue (prefix + "Text", prefix);
            
            auto textLength = (int) text.getNumBytesAsUTF8() + 1; // include null terminator
            auto chunkLength = textLength + 20 + (textLength & 1);
            
            out.writeInt (chunkName ("ltxt"));
            out.writeInt (chunkLength);
            out.writeInt (getValue (values, prefix, "Identifier"));
            out.writeInt (getValue (values, prefix, "SampleLength"));
            out.writeInt (getValue (values, prefix, "Purpose"));
            out.writeShort ((short) getValue (values, prefix, "Country"));
            out.writeShort ((short) getValue (values, prefix, "Language"));
            out.writeShort ((short) getValue (values, prefix, "Dialect"));
            out.writeShort ((short) getValue (values, prefix, "CodePage"));
            out.write (text.toUTF8(), (size_t) textLength);
            
            if ((out.getDataSize() & 1) != 0)
                out.writeByte (0);
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            auto numCueLabels  = getValue (values, "NumCueLabels");
            auto numCueNotes   = getValue (values, "NumCueNotes");
            auto numCueRegions = getValue (values, "NumCueRegions");
            
            MemoryOutputStream out;
            
            if (numCueLabels + numCueNotes + numCueRegions > 0)
            {
                out.writeInt (chunkName ("adtl"));
                
                for (int i = 0; i < numCueLabels; ++i)
                    appendLabelOrNoteChunk (values, "CueLabel" + String (i), chunkName ("labl"), out);
                
                for (int i = 0; i < numCueNotes; ++i)
                    appendLabelOrNoteChunk (values, "CueNote" + String (i), chunkName ("note"), out);
                
                for (int i = 0; i < numCueRegions; ++i)
                    appendExtraChunk (values, "CueRegion" + String (i), out);
            }
            
            return out.getMemoryBlock();
        }
    }
    
    //==============================================================================
    /** Reads a RIFF List Info chunk from a stream positioned just after the size byte. */
    namespace ListInfoChunk
    {
        static const char* const types[] =
        {
            WavAudioFormat::riffInfoArchivalLocation,
            WavAudioFormat::riffInfoArtist,
            WavAudioFormat::riffInfoBaseURL,
            WavAudioFormat::riffInfoCinematographer,
            WavAudioFormat::riffInfoComment,
            WavAudioFormat::riffInfoComments,
            WavAudioFormat::riffInfoComment2,
            WavAudioFormat::riffInfoCommissioned,
            WavAudioFormat::riffInfoCopyright,
            WavAudioFormat::riffInfoCostumeDesigner,
            WavAudioFormat::riffInfoCountry,
            WavAudioFormat::riffInfoCropped,
            WavAudioFormat::riffInfoDateCreated,
            WavAudioFormat::riffInfoDateTimeOriginal,
            WavAudioFormat::riffInfoDefaultAudioStream,
            WavAudioFormat::riffInfoDimension,
            WavAudioFormat::riffInfoDirectory,
            WavAudioFormat::riffInfoDistributedBy,
            WavAudioFormat::riffInfoDotsPerInch,
            WavAudioFormat::riffInfoEditedBy,
            WavAudioFormat::riffInfoEighthLanguage,
            WavAudioFormat::riffInfoEncodedBy,
            WavAudioFormat::riffInfoEndTimecode,
            WavAudioFormat::riffInfoEngineer,
            WavAudioFormat::riffInfoFifthLanguage,
            WavAudioFormat::riffInfoFirstLanguage,
            WavAudioFormat::riffInfoFourthLanguage,
            WavAudioFormat::riffInfoGenre,
            WavAudioFormat::riffInfoKeywords,
            WavAudioFormat::riffInfoLanguage,
            WavAudioFormat::riffInfoLength,
            WavAudioFormat::riffInfoLightness,
            WavAudioFormat::riffInfoLocation,
            WavAudioFormat::riffInfoLogoIconURL,
            WavAudioFormat::riffInfoLogoURL,
            WavAudioFormat::riffInfoMedium,
            WavAudioFormat::riffInfoMoreInfoBannerImage,
            WavAudioFormat::riffInfoMoreInfoBannerURL,
            WavAudioFormat::riffInfoMoreInfoText,
            WavAudioFormat::riffInfoMoreInfoURL,
            WavAudioFormat::riffInfoMusicBy,
            WavAudioFormat::riffInfoNinthLanguage,
            WavAudioFormat::riffInfoNumberOfParts,
            WavAudioFormat::riffInfoOrganisation,
            WavAudioFormat::riffInfoPart,
            WavAudioFormat::riffInfoProducedBy,
            WavAudioFormat::riffInfoProductName,
            WavAudioFormat::riffInfoProductionDesigner,
            WavAudioFormat::riffInfoProductionStudio,
            WavAudioFormat::riffInfoRate,
            WavAudioFormat::riffInfoRated,
            WavAudioFormat::riffInfoRating,
            WavAudioFormat::riffInfoRippedBy,
            WavAudioFormat::riffInfoSecondaryGenre,
            WavAudioFormat::riffInfoSecondLanguage,
            WavAudioFormat::riffInfoSeventhLanguage,
            WavAudioFormat::riffInfoSharpness,
            WavAudioFormat::riffInfoSixthLanguage,
            WavAudioFormat::riffInfoSoftware,
            WavAudioFormat::riffInfoSoundSchemeTitle,
            WavAudioFormat::riffInfoSource,
            WavAudioFormat::riffInfoSourceFrom,
            WavAudioFormat::riffInfoStarring_ISTR,
            WavAudioFormat::riffInfoStarring_STAR,
            WavAudioFormat::riffInfoStartTimecode,
            WavAudioFormat::riffInfoStatistics,
            WavAudioFormat::riffInfoSubject,
            WavAudioFormat::riffInfoTapeName,
            WavAudioFormat::riffInfoTechnician,
            WavAudioFormat::riffInfoThirdLanguage,
            WavAudioFormat::riffInfoTimeCode,
            WavAudioFormat::riffInfoTitle,
            WavAudioFormat::riffInfoTrackNo,
            WavAudioFormat::riffInfoTrackNumber,
            WavAudioFormat::riffInfoURL,
            WavAudioFormat::riffInfoVegasVersionMajor,
            WavAudioFormat::riffInfoVegasVersionMinor,
            WavAudioFormat::riffInfoVersion,
            WavAudioFormat::riffInfoWatermarkURL,
            WavAudioFormat::riffInfoWrittenBy,
            WavAudioFormat::riffInfoYear
        };
        
        static bool isMatchingTypeIgnoringCase (const int value, const char* const name) noexcept
        {
            for (int i = 0; i < 4; ++i)
                if ((juce_wchar) name[i] != CharacterFunctions::toUpperCase ((juce_wchar) ((value >> (i * 8)) & 0xff)))
                    return false;
            
            return true;
        }
        
        static void addToMetadata (StringPairArray& values, InputStream& input, int64 chunkEnd)
        {
            while (input.getPosition() < chunkEnd)
            {
                auto infoType = input.readInt();
                auto infoLength = chunkEnd - input.getPosition();
                
                if (infoLength > 0)
                {
                    infoLength = jmin (infoLength, (int64) input.readInt());
                    
                    if (infoLength <= 0)
                        return;
                    
                    for (auto& type : types)
                    {
                        if (isMatchingTypeIgnoringCase (infoType, type))
                        {
                            MemoryBlock mb;
                            input.readIntoMemoryBlock (mb, (ssize_t) infoLength);
                            values.set (type, String::createStringFromData ((const char*) mb.getData(),
                                                                            (int) mb.getSize()));
                            break;
                        }
                    }
                }
            }
        }
        
        static bool writeValue (const StringPairArray& values, MemoryOutputStream& out, const char* paramName)
        {
            auto value = values.getValue (paramName, {});
            
            if (value.isEmpty())
                return false;
            
            auto valueLength = (int) value.getNumBytesAsUTF8() + 1;
            auto chunkLength = valueLength + (valueLength & 1);
            
            out.writeInt (chunkName (paramName));
            out.writeInt (chunkLength);
            out.write (value.toUTF8(), (size_t) valueLength);
            
            if ((out.getDataSize() & 1) != 0)
                out.writeByte (0);
            
            return true;
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            MemoryOutputStream out;
            out.writeInt (chunkName ("INFO"));
            bool anyParamsDefined = false;
            
            for (auto& type : types)
                if (writeValue (values, out, type))
                    anyParamsDefined = true;
            
            return anyParamsDefined ? out.getMemoryBlock() : MemoryBlock();
        }
    }
    
    //==============================================================================
    struct AcidChunk
    {
        /** Reads an acid RIFF chunk from a stream positioned just after the size byte. */
        AcidChunk (InputStream& input, size_t length)
        {
            zerostruct (*this);
            input.read (this, (int) jmin (sizeof (*this), length));
        }
        
        AcidChunk (const StringPairArray& values)
        {
            zerostruct (*this);
            
            flags = getFlagIfPresent (values, WavAudioFormat::acidOneShot,   0x01)
            | getFlagIfPresent (values, WavAudioFormat::acidRootSet,   0x02)
            | getFlagIfPresent (values, WavAudioFormat::acidStretch,   0x04)
            | getFlagIfPresent (values, WavAudioFormat::acidDiskBased, 0x08)
            | getFlagIfPresent (values, WavAudioFormat::acidizerFlag,  0x10);
            
            if (values[WavAudioFormat::acidRootSet].getIntValue() != 0)
                rootNote = ByteOrder::swapIfBigEndian ((uint16) values[WavAudioFormat::acidRootNote].getIntValue());
            
            numBeats          = ByteOrder::swapIfBigEndian ((uint32) values[WavAudioFormat::acidBeats].getIntValue());
            meterDenominator  = ByteOrder::swapIfBigEndian ((uint16) values[WavAudioFormat::acidDenominator].getIntValue());
            meterNumerator    = ByteOrder::swapIfBigEndian ((uint16) values[WavAudioFormat::acidNumerator].getIntValue());
            
            if (values.containsKey (WavAudioFormat::acidTempo))
                tempo = swapFloatByteOrder (values[WavAudioFormat::acidTempo].getFloatValue());
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            return AcidChunk (values).toMemoryBlock();
        }
        
        MemoryBlock toMemoryBlock() const
        {
            return (flags != 0 || rootNote != 0 || numBeats != 0 || meterDenominator != 0 || meterNumerator != 0)
            ? MemoryBlock (this, sizeof (*this)) : MemoryBlock();
        }
        
        void addToMetadata (StringPairArray& values) const
        {
            setBoolFlag (values, WavAudioFormat::acidOneShot,   0x01);
            setBoolFlag (values, WavAudioFormat::acidRootSet,   0x02);
            setBoolFlag (values, WavAudioFormat::acidStretch,   0x04);
            setBoolFlag (values, WavAudioFormat::acidDiskBased, 0x08);
            setBoolFlag (values, WavAudioFormat::acidizerFlag,  0x10);
            
            if (flags & 0x02) // root note set
                values.set (WavAudioFormat::acidRootNote, String (ByteOrder::swapIfBigEndian (rootNote)));
            
            values.set (WavAudioFormat::acidBeats,       String (ByteOrder::swapIfBigEndian (numBeats)));
            values.set (WavAudioFormat::acidDenominator, String (ByteOrder::swapIfBigEndian (meterDenominator)));
            values.set (WavAudioFormat::acidNumerator,   String (ByteOrder::swapIfBigEndian (meterNumerator)));
            values.set (WavAudioFormat::acidTempo,       String (swapFloatByteOrder (tempo)));
        }
        
        void setBoolFlag (StringPairArray& values, const char* name, uint32 mask) const
        {
            values.set (name, (flags & ByteOrder::swapIfBigEndian (mask)) ? "1" : "0");
        }
        
        static uint32 getFlagIfPresent (const StringPairArray& values, const char* name, uint32 flag)
        {
            return values[name].getIntValue() != 0 ? ByteOrder::swapIfBigEndian (flag) : 0;
        }
        
        static float swapFloatByteOrder (const float x) noexcept
        {
#ifdef JUCE_BIG_ENDIAN
            union { uint32 asInt; float asFloat; } n;
            n.asFloat = x;
            n.asInt = ByteOrder::swap (n.asInt);
            return n.asFloat;
#else
            return x;
#endif
        }
        
        uint32 flags;
        uint16 rootNote;
        uint16 reserved1;
        float reserved2;
        uint32 numBeats;
        uint16 meterDenominator;
        uint16 meterNumerator;
        float tempo;
        
    } JUCE_PACKED;
    
    //==============================================================================
    struct TracktionChunk
    {
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            MemoryOutputStream out;
            auto s = values[WavAudioFormat::tracktionLoopInfo];
            
            if (s.isNotEmpty())
            {
                out.writeString (s);
                
                if ((out.getDataSize() & 1) != 0)
                    out.writeByte (0);
            }
            
            return out.getMemoryBlock();
        }
    };
    
    //==============================================================================
    namespace AXMLChunk
    {
        static void addToMetadata (StringPairArray& destValues, const String& source)
        {
            if (auto xml = parseXML (source))
            {
                if (xml->hasTagName ("ebucore:ebuCoreMain"))
                {
                    if (auto xml2 = xml->getChildByName ("ebucore:coreMetadata"))
                    {
                        if (auto xml3 = xml2->getChildByName ("ebucore:identifier"))
                        {
                            if (auto xml4 = xml3->getChildByName ("dc:identifier"))
                            {
                                auto ISRCCode = xml4->getAllSubText().fromFirstOccurrenceOf ("ISRC:", false, true);
                                
                                if (ISRCCode.isNotEmpty())
                                    destValues.set (WavAudioFormat::ISRC, ISRCCode);
                            }
                        }
                    }
                }
            }
        }
        
        static MemoryBlock createFrom (const StringPairArray& values)
        {
            auto ISRC = values.getValue (WavAudioFormat::ISRC, {});
            MemoryOutputStream xml;
            
            if (ISRC.isNotEmpty())
            {
                xml << "<ebucore:ebuCoreMain xmlns:dc=\" http://purl.org/dc/elements/1.1/\" "
                "xmlns:ebucore=\"urn:ebu:metadata-schema:ebuCore_2012\">"
                "<ebucore:coreMetadata>"
                "<ebucore:identifier typeLabel=\"GUID\" "
                "typeDefinition=\"Globally Unique Identifier\" "
                "formatLabel=\"ISRC\" "
                "formatDefinition=\"International Standard Recording Code\" "
                "formatLink=\"http://www.ebu.ch/metadata/cs/ebu_IdentifierTypeCodeCS.xml#3.7\">"
                "<dc:identifier>ISRC:" << ISRC << "</dc:identifier>"
                "</ebucore:identifier>"
                "</ebucore:coreMetadata>"
                "</ebucore:ebuCoreMain>";
                
                xml.writeRepeatedByte (0, xml.getDataSize());  // ensures even size, null termination and room for future growing
            }
            
            return xml.getMemoryBlock();
        }
    }
    
    //==============================================================================
    struct ExtensibleWavSubFormat
    {
        uint32 data1;
        uint16 data2;
        uint16 data3;
        uint8  data4[8];
        
        bool operator== (const ExtensibleWavSubFormat& other) const noexcept   { return memcmp (this, &other, sizeof (*this)) == 0; }
        bool operator!= (const ExtensibleWavSubFormat& other) const noexcept   { return ! operator== (other); }
        
    } JUCE_PACKED;
    
    static const ExtensibleWavSubFormat pcmFormat       = { 0x00000001, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
    static const ExtensibleWavSubFormat IEEEFloatFormat = { 0x00000003, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
    static const ExtensibleWavSubFormat ambisonicFormat = { 0x00000001, 0x0721, 0x11d3, { 0x86, 0x44, 0xC8, 0xC1, 0xCA, 0x00, 0x00, 0x00 } };
    
    struct DataSize64Chunk   // chunk ID = 'ds64' if data size > 0xffffffff, 'JUNK' otherwise
    {
        uint32 riffSizeLow;     // low 4 byte size of RF64 block
        uint32 riffSizeHigh;    // high 4 byte size of RF64 block
        uint32 dataSizeLow;     // low 4 byte size of data chunk
        uint32 dataSizeHigh;    // high 4 byte size of data chunk
        uint32 sampleCountLow;  // low 4 byte sample count of fact chunk
        uint32 sampleCountHigh; // high 4 byte sample count of fact chunk
        uint32 tableLength;     // number of valid entries in array 'table'
    } JUCE_PACKED;
    
#if JUCE_MSVC
#pragma pack (pop)
#endif
}

} // end namespace juce



#include <Processors/Formats/Impl/HashOutputFormat.h>
#include <Columns/IColumn.h>
#include <Formats/FormatFactory.h>
#include <Processors/Port.h>

#include <city.h>
#include <MurmurHash3.h>
#include <xxhash.h>

namespace DB
{

HashOutputFormat::HashOutputFormat(WriteBuffer & out_, SharedHeader header_, const FormatSettings & format_settings_)
    : IOutputFormat(header_, out_), format_settings(format_settings_)
{
}

HashOutputFormat::~HashOutputFormat()
{
    buf.cancel();
}

String HashOutputFormat::getName() const
{
    return "HashOutputFormat";
}

void HashOutputFormat::consume(Chunk chunk)
{
    // TODO - hide arena declarion inside of non-SIP scope
    Arena arena;
    const char * begin = nullptr;

    for (const auto & column : chunk.getColumns())
    {
        for (size_t i = 0; i < column->size(); ++i)

            switch (format_settings.hash.function) {

                case FormatSettings::HashEnumFunction::SIP_HASH_64:
                case FormatSettings::HashEnumFunction::SIP_HASH_128:
                    column->updateHashWithValue(i, sip);
                    break;

                default:
                {
                    StringRef ref = column->serializeValueIntoArena(i, arena, begin);
                    buf.write(ref.data, ref.size);
                }
            }
    }
}

void HashOutputFormat::finalizeImpl()
{
    std::string hashString;
    switch (format_settings.hash.function) {

        case FormatSettings::HashEnumFunction::SIP_HASH_128:
            hashString = getSipHash128AsHexString(sip);
            break;

        case FormatSettings::HashEnumFunction::SIP_HASH_64:
            hashString = getHexUIntLowercase(sip.get64());
            break;

        case FormatSettings::HashEnumFunction::CITY_HASH_128: 
        {
            CityHash_v1_0_2::uint128 hash = CityHash_v1_0_2::CityHash128(buf.buffer().begin(), buf.buffer().size());
            hashString += getHexUIntLowercase(hash.high64);
            hashString += getHexUIntLowercase(hash.low64);
            break;
        }

        case FormatSettings::HashEnumFunction::CITY_HASH_64:
            hashString = getHexUIntLowercase(CityHash_v1_0_2::CityHash64(buf.buffer().begin(), buf.buffer().size()));
            break;

        case FormatSettings::HashEnumFunction::MURMUR_HASH3_128:
        {
            HashState bytes;
            MurmurHash3_x64_128(buf.buffer().begin(), buf.buffer().size(), 0, bytes);
            hashString += getHexUIntLowercase(bytes.h1);
            hashString += getHexUIntLowercase(bytes.h2);
            break;
        }

        case FormatSettings::HashEnumFunction::MURMUR_HASH3_64:
        {
            HashState bytes;
            MurmurHash3_x64_128(buf.buffer().begin(), buf.buffer().size(), 0, bytes);
            hashString = getHexUIntLowercase(bytes.h1 ^ bytes.h2);
            break;
        }

        case FormatSettings::HashEnumFunction::XX_HASH_64:
        {
            UInt64 hash = XXH64(buf.buffer().begin(), buf.buffer().size(), 0);
            hashString = getHexUIntLowercase(hash);
            break;
        }
    }

    out.write(hashString.data(), hashString.size());
    out.write("\n", 1);
    out.next();
}

void registerOutputFormatHash(FormatFactory & factory)
{
    factory.registerOutputFormat("Hash",
        [](WriteBuffer & buf, const Block & header, const FormatSettings & format_settings)
        {
            return std::make_shared<HashOutputFormat>(buf, std::make_shared<const Block>(header), format_settings);
        });
}

}

#include <Processors/Formats/Impl/HashOutputFormat.h>
#include <Columns/IColumn.h>
#include <Formats/FormatFactory.h>
#include <IO/WriteBuffer.h>
#include <Processors/Port.h>


namespace DB
{

HashOutputFormat::HashOutputFormat(WriteBuffer & out_, SharedHeader header_)
    : IOutputFormat(header_, out_) 
{
}

String HashOutputFormat::getName() const
{
    return "HashOutputFormat";
}

void HashOutputFormat::consume(Chunk chunk)
{
    for (const auto & column : chunk.getColumns())
    {
        for (size_t i = 0; i < column->size(); ++i)
            column->updateHashWithValue(i, hash);
    }
}

void HashOutputFormat::finalizeImpl()
{
    std::string hashString = getSipHash128AsHexString(hash);
    out.write(hashString.data(), hashString.size());
    out.write("\n", 1);
    out.next();
}

void registerOutputFormatHash(FormatFactory & factory)
{
    factory.registerOutputFormat("Hash",
        [](WriteBuffer & buf, const Block & header, const FormatSettings &)
        {
            return std::make_shared<HashOutputFormat>(buf, std::make_shared<const Block>(header));
        });
}

}

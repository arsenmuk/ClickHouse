#pragma once

#include <Common/SipHash.h>
#include <Processors/Formats/IOutputFormat.h>

namespace DB
{

class HashOutputFormat final  : public IOutputFormat
{
public:
    HashOutputFormat(WriteBuffer & out_, SharedHeader header_);
    String getName() const override;

private:
    void consume(Chunk chunk) override;
    void finalizeImpl() override;

    SipHash hash;
};

}

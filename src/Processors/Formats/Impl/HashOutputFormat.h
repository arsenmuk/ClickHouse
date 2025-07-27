#pragma once

#include <Common/SipHash.h>
#include <Formats/FormatSettings.h>
#include <IO/BufferWithOwnMemory.h>
#include <IO/WriteBuffer.h>
#include <Processors/Formats/IOutputFormat.h>

namespace DB
{

class HashOutputFormat final : public IOutputFormat
{
public:
    HashOutputFormat(WriteBuffer & out_, SharedHeader header_, const FormatSettings & format_settings_);
    ~HashOutputFormat() override;

    String getName() const override;

private:
    void consume(Chunk chunk) override;
    void finalizeImpl() override;

    // TODO - check whether global format settings object can mutate during lifecyle, is it safe to keep a reference instead of copy?
    const FormatSettings & format_settings;

    // TODO - make as union, as we don't need them all at once
    SipHash sip;
    BufferWithOwnMemory<WriteBuffer> buf;
};

}

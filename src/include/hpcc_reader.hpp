#pragma once

#include "duckdb/common/helper.hpp"
#include "duckdb/common/multi_file/base_file_reader.hpp"

namespace duckdb {

class HPCCReader : public BaseFileReader {
public:
	HPCCReader(ClientContext &context, const OpenFileInfo file);

	~HPCCReader() {
		// hpcc_value_decref(&value);
		// hpcc_file_reader_close(reader);
	}

public:
	void Read(DataChunk &output);

	string GetReaderType() const override {
		return "hpcc";
	}

	bool TryInitializeScan(ClientContext &context, GlobalTableFunctionState &gstatep,
	                       LocalTableFunctionState &lstate) override;
	void Scan(ClientContext &context, GlobalTableFunctionState &global_state, LocalTableFunctionState &local_state,
	          DataChunk &chunk) override;

public:
	// hpcc_file_reader_t reader;
	// hpcc_value_t value;
	unique_ptr<Vector> read_vec;

	BufferHandle buf_handle;
	// HPCCType hpcc_type;
	LogicalType duckdb_type;
};

} // namespace duckdb
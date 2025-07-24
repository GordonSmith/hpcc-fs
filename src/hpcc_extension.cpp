#define DUCKDB_EXTENSION_MAIN

#include "hpcc_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void HpccScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "hpcc " + name.GetString() + " 🐥");
	});
}

inline void HpccOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "hpcc " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(DatabaseInstance &instance) {
	// Register a scalar function
	auto hpcc_scalar_function = ScalarFunction("hpcc", {LogicalType::VARCHAR}, LogicalType::VARCHAR, HpccScalarFun);
	ExtensionUtil::RegisterFunction(instance, hpcc_scalar_function);

	// Register another scalar function
	auto hpcc_openssl_version_scalar_function = ScalarFunction("hpcc_openssl_version", {LogicalType::VARCHAR},
	                                                           LogicalType::VARCHAR, HpccOpenSSLVersionScalarFun);
	ExtensionUtil::RegisterFunction(instance, hpcc_openssl_version_scalar_function);
}

void HpccExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string HpccExtension::Name() {
	return "hpcc";
}

std::string HpccExtension::Version() const {
#ifdef EXT_VERSION_HPCC
	return EXT_VERSION_HPCC;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void hpcc_init(duckdb::DatabaseInstance &db) {
	duckdb::DuckDB db_wrapper(db);
	db_wrapper.LoadExtension<duckdb::HpccExtension>();
}

DUCKDB_EXTENSION_API const char *hpcc_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif

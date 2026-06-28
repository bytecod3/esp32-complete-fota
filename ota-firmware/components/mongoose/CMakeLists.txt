idf_component_register(
	SRCS mongoose.c
    INCLUDE_DIRS include
    REQUIRES app_update esp_timer mbedtls
)

target_compile_definitions(${COMPONENT_LIB}
    PRIVATE
        MG_TLS=MG_TLS_MBED
)    
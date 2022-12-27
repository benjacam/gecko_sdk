add_library(gecko_emlib INTERFACE)

set(SRC_LIST em_acmp em_adc em_aes em_burtc em_can em_cmu em_core em_cryotimer em_crypto em_csen em_dac em_dbg em_ebi em_emu em_eusart em_gpcrc em_gpio em_i2c em_iadc em_idac em_lcd em_ldma em_lesense em_letimer em_leuart em_msc em_opamp em_pcnt em_pdm em_prs em_qspi em_rmu em_rtc em_rtcc em_se em_system em_timer em_usart em_vcmp em_vdac em_wdog)
list(TRANSFORM SRC_LIST PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(TRANSFORM SRC_LIST APPEND ".c")

target_sources(gecko_emlib INTERFACE ${SRC_LIST})
target_include_directories(gecko_emlib INTERFACE ${CMAKE_CURRENT_LIST_DIR}/inc)

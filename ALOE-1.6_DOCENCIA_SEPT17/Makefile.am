
SUBDIRS=aloeSKs/aloeSK15/skel_SK15/lnx_make utils/lnx_make hw_api/lnx sw_daemons/lnx_make cmdman_console sw_api/lnx_make modules/typetools/lnx_make modules/CHANNEL_ANALYZER/lnx_make/ modules/channel_noise/lnx_make/ modules/CP/lnx_make/ modules/CPLX_FILTER/lnx_make/ modules/CRC/lnx_make/ modules/CTRL_example2/lnx_make/ modules/DAC_JACK0/lnx_make/ modules/data_source/lnx_make/ modules/data_source_sink/lnx_make/ modules/DCP/lnx_make/ modules/DDC/lnx_make/ modules/deMAPPINGII/lnx_make/ modules/demod16QAM/lnx_make/ modules/DUC17/lnx_make/ modules/DUCt16/lnx_make/ modules/fft_ifft/lnx_make/ modules/GRAPH/lnx_make/ modules/MAPPINGIII/lnx_make/ modules/mod16QAM/lnx_make/ modules/module_template/lnx_make/ modules/module_template4/lnx_make/ modules/PROVA/lnx_make/ modules/REAL_FILTER/lnx_make/ modules/sink_graph/lnx_make/ modules/sink_plot/lnx_make/ modules/TURBODUMMY/lnx_make/ 

EXTRA_DIST=scripts doc example-repository aloeui matlab aloeui.run

dist_bin_SCRIPTS=scripts/viewlogs.sh scripts/update_modules.pl aloeui.run

dist-hook:
	rm -rf `find $(distdir)/ -name .svn`
	rm -rf `find $(distdir)/ -name .deps`
	rm -rf `find $(distdir)/ -name .libs`
	rm -rf `find $(distdir)/ -name bin_*`
	rm -rf `find $(distdir)/ -name lib_*`
	rm -rf `find $(distdir)/ -name ._*`
	rm -rf `find $(distdir)/ -name .dirstamp`
	
	
	
	

#######################################################################data_source_sink
object {
	obj_name=DATASOURCESINK
	exe_name=data_source_sink

	inputs {
		name=input_1
		remote_itf=output_1
		remote_obj=MAPPINGIII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=MAPPINGIII
	}
}
#######################################################################data_source_sink
#######################################################################MAPPINGIII
object {
	obj_name=MAPPINGIII
	exe_name=MAPPINGIII
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=DATASOURCESINK
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=ifft1
	}
	outputs {
		name=output_1
		remote_itf=input_1
		remote_obj=DATASOURCESINK
	}
}
#######################################################################MAPPINGIII
#######################################################################ifft1
object {
	obj_name=ifft1
	exe_name=fft_ifft
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=MAPPINGIII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=fft
	}
}
#######################################################################ifft1
#######################################################################fft

object {
	obj_name=fft
	exe_name=fft_ifft
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=ifft1
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=deMAPPINGII
	}
}
#######################################################################fft
#######################################################################deMAPPINGII
	

object {
	obj_name=deMAPPINGII
	exe_name=deMAPPINGII
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=fft
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=GRAPH
	}
}

#######################################################################deMAPPINGII
#######################################################################GRAPH

object {
	obj_name=GRAPH
	exe_name=GRAPH
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=deMAPPINGII
	}

}
#######################################################################GRAPH

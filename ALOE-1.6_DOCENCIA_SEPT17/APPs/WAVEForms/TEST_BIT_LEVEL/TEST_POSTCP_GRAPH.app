#######################################################################DATASOURCESINK
object {
	obj_name=DATASOURCESINK
	exe_name=data_source_sink
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=MAPPINGIII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=CRC
	}
}
#######################################################################DATASOURCESINK
#######################################################################CRC
object {
	obj_name=CRC
	exe_name=CRC
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=DATASOURCESINK
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=TURBOCODER
	}
}
#######################################################################CRC
#######################################################################TURBOCODER
object {
	obj_name=TURBOCODER
	exe_name=TURBODUMMY
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=CRC
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=mod16QAM
	}
}
#######################################################################TURBOCODER
#######################################################################mod16QAM

object {
	obj_name=mod16QAM
	exe_name=mod16QAM
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=TURBOCODER
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=MAPPINGIII
	}
}
#######################################################################mod16QAM
#######################################################################MAPPINGIII

object {
	obj_name=MAPPINGIII
	exe_name=MAPPINGIII
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=mod16QAM
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=ifftI
	}
}
#######################################################################MAPPINGIII
#######################################################################ifftI


object {
	obj_name=ifftI
	exe_name=fft_ifft
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=MAPPINGIII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=CP
	}
}
#######################################################################ifftI
#######################################################################CP
object {
	obj_name=CP
	exe_name=CP
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=ifftI
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=GRAPH
	}
}
#######################################################################CP
#######################################################################GRAPH
object {
	obj_name=GRAPH
	exe_name=GRAPH
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=CP
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=DCP
	}
}
#######################################################################GRAPH
#######################################################################DCP
object {
	obj_name=DCP
	exe_name=DCP
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=GRAPH
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=fft
	}
}
#######################################################################DCP
#######################################################################fft
object {
	obj_name=fft
	exe_name=fft_ifft
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=DCP
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=deMAPPINGII
	}
}

#######################################################################fft
#######################################################################DEMAPP
object {
	obj_name=deMAPPINGII
	exe_name=MAPPINGIII
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=fft
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=demod16QAM
	}
}

#######################################################################DEMAPP
#######################################################################Demod
object {
	obj_name=demod16QAM
	exe_name=demod16QAM
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=deMAPPINGII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=TURBODECODER
	}
}

#######################################################################Demod
#######################################################################TURBODECODER
object {
	obj_name=TURBODECODER
	exe_name=TURBODUMMY
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=demod16QAM
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=UNCRC
	}
}

#######################################################################TURBODECODER
#######################################################################UNCRC
object {
	obj_name=UNCRC
	exe_name=CRC
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=TURBODECODER
	}
}
#######################################################################UNCRC

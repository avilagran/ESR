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
		remote_obj=CRC
	}
}
#######################################################################data_source_sink
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
#######################################################################CRC
#######################################################################UNCRC

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
#######################################################################CRC
#######################################################################UNCRC

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
		remote_obj=deMAPPINGII
	}
	outputs {
		name=output_1
		remote_itf=input_1
		remote_obj=DATASOURCESINK
	}
#######################################################################CRC
#######################################################################UNCRC
	
}
object {
	obj_name=deMAPPINGII
	exe_name=deMAPPINGII
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=MAPPINGIII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=GRAPH
	}
}
#######################################################################CRC
#######################################################################UNCRC
object {
	obj_name=GRAPH
	exe_name=GRAPH
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=deMAPPINGIII
	}
	
}

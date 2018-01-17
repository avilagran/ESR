#######################################################################data_source_sink
object {
	obj_name=data_source_sink
	exe_name=data_source_sink
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=UNCRC
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
		remote_obj=data_source_sink
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
		remote_obj=MAPINGIII
	}
}
#######################################################################CRC
#######################################################################UNCRC

object {
	obj_name=MAPINGIII
	exe_name=MAPINGIII
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=mod16QAM
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=channel_noise
	}
	
}
#######################################################################CRC
#######################################################################UNCRC

object {
	obj_name=channel_noise
	exe_name=channel_noise
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=MAPINGIII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=deMAPINGIII
	}
}
#######################################################################CRC
#######################################################################UNCRC


object {
	obj_name=deMAPINGIII
	exe_name=MAPINGIII
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=channel_noise
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=demod16QAM
	}
}
#######################################################################UNCRC
#######################################################################UNCRC


object {
	obj_name=demod16QAM
	exe_name=demod16QAM
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=deMAPINGIII
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=TURBODECODER
	}
}
#######################################################################UNCRC
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
#######################################################################UNCRC
#######################################################################UNCRC
object {
	obj_name=UNCRC
	exe_name=CRC
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=TURBODECODER
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=data_source_sink
	}
}
#######################################################################UNCRC


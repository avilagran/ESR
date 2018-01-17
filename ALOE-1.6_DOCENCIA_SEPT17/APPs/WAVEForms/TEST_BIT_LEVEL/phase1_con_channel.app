#######################################################################DATASOURCESINK
object {
	obj_name=DATASOURCESINK
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
		remote_obj=mod16QAM
	}
}








#######################################################################TURBOCODER
#######################################################################mod16QAM
#######################################################################GRAPH
object {
	obj_name=mod16QAM
	exe_name=mod16QAM

	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=CRC
		}


	
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=GRAPH
	}
}









#######################################################################GRAPH
#######################################################################Demod16QAM
object {
	obj_name=GRAPH
	exe_name=GRAPH
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



#######################################################################channel_noise
#######################################################################Demod16QAM
object {
	obj_name=channel_noise
	exe_name=channel_noise
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=GRAPH
	}
	
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=demod16QAM
	}


}












#######################################################################mod16QAM
#######################################################################demod16QAM
object {
	obj_name=demod16QAM
	exe_name=demod16QAM

	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=channel_noise
		}


	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=UNCRC
	}
}



#######################################################################CRC
#######################################################################UNCRC
object {
	obj_name=UNCRC
	exe_name=CRC
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=demod16QAM
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=DATASOURCESINK
	}
}



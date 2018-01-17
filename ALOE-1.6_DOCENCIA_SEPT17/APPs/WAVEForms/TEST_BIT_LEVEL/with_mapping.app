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
#######################################################################mod16QAMH
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

#######################################################################mod16QAMH
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
		remote_obj=DUC17
	}


}




#######################################################################ifftI
#######################################################################DUC17
object {
	obj_name=DUC17
	exe_name=DUC17
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=ifftI
	}
	
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=channel_noise
	}


}





#######################################################################DUC17
#######################################################################channel_noise
object {
	obj_name=channel_noise
	exe_name=channel_noise
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=DUC17
	}
	
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=DDC
	}


}



#######################################################################channel_noise
#######################################################################DDC
object {
	obj_name=DDC
	exe_name=DDC

	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=channel_noise
		}


	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=fft
	}
}








#######################################################################DDC
#######################################################################fft

object {
	obj_name=fft
	exe_name=fft_ifft
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=DDC
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
		remote_obj=mod16QAM
	}
	
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=demod16QAM
	}


}



#######################################################################deMAPPINGII
#######################################################################demod16QAM
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
		remote_obj=TURBOCODER
	}
}

#######################################################################demod16QAM
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


#######################################################################TURBOCODER
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
		remote_obj=DATASOURCESINK
	}
}



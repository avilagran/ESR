
object {
	obj_name=data_source_sink 
	exe_name=data_source_sink 
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=demod16QAM
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=mod16QAM
	}
}

object {
	obj_name=mod16QAM
	exe_name=mod16QAM
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=data_source_sink
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=demod16QAM
	}
}


object {
	obj_name=demod16QAM
	exe_name=demod16QAM
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=mod16QAM
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=data_source_sink
	}
}



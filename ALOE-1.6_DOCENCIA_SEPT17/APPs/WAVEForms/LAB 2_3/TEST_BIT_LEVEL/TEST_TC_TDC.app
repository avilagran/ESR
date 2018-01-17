#######################################################################TURBOCODER
object {
	obj_name=TURBOCODER
	exe_name=TURBOCODER
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=TURBODECODER
	}
}
#######################################################################TURBODECODER
object {
	obj_name=TURBODECODER
	exe_name=TURBODECODER
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=TURBOCODER
	}
}


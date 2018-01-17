#######################################################################
object {
	obj_name=MAPPINGIII
	exe_name=MAPPINGIII
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=GRAPH
	}
}
#######################################################################
object {
	obj_name=GRAPH
	exe_name=GRAPH
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=MAPPINGIII
	}
}


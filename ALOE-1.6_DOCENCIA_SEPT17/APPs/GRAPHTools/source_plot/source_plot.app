object {
	obj_name=data_source
	exe_name=data_source
	kopts=240
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=sink_plot
		kbpts=8000.3
	}
}
#######################################################################DATASOURCE
####################################################################SINK_plot
object {
	obj_name=sink_plot				
	exe_name=sink_plot		
	kopts=2500
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=data_source
	}
}

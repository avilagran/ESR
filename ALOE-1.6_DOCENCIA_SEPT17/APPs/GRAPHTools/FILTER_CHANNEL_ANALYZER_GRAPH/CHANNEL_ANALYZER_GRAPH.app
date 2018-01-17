
#########################################################################DAC_JACK0
#########################################################################CHANNEL_ANALYZER
object {
	obj_name=CHANNEL_ANALYZER				
	exe_name=CHANNEL_ANALYZER			

	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=GRAPH
	}
}
#########################################################################CHANNEL_ANALYZER
#########################################################################GRAPH
object {
	obj_name=GRAPH				
	exe_name=GRAPH			
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=CHANNEL_ANALYZER
	}
}

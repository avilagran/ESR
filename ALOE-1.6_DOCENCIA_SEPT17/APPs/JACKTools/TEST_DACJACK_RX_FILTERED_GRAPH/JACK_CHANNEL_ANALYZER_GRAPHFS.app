object {
	obj_name=DAC_JACK0
	exe_name=DAC_JACK0
#	force_pe=0
	kopts=15
	inputs {
		name=input
		remote_itf=output_0
		remote_obj=INPUTFS_GRAPH
	}
	outputs {
		name=output
		remote_itf=input_0
		remote_obj=CHANNEL_FS
	}
}
#########################################################################DAC_JACK0
#########################################################################CHANNEL_ANALYZER
object {
	obj_name=CHANNEL_FS				
	exe_name=CHANNEL_ANALYZER			
	inputs {
		name=input_0
		remote_itf=output
		remote_obj=DAC_JACK0
	}
	outputs {
		name=output_0
		remote_itf=input_0
		remote_obj=INPUTFS_GRAPH
	}
	outputs {
		name=output_1
		remote_itf=input_0
		remote_obj=FS_GRAPH
	}
}
#########################################################################CHANNEL_ANALYZER
#########################################################################GRAPH
object {
	obj_name=FS_GRAPH				
	exe_name=GRAPH			
	inputs {
		name=input_0
		remote_itf=output_1
		remote_obj=CHANNEL_FS
	}
}
#########################################################################CHANNEL_ANALYZER
#########################################################################GRAPH
object {
	obj_name=INPUTFS_GRAPH				
	exe_name=GRAPH			
	inputs {
		name=input_0
		remote_itf=output_0
		remote_obj=CHANNEL_FS
	}
	outputs {
		name=output_0
		remote_itf=input
		remote_obj=DAC_JACK0
	}
}

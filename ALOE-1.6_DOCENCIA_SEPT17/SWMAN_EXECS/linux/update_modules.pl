#!/usr/bin/perl

$makefile = "Makefile.am";
$base_subdirs = "aloeSKs/aloeSK15/skel_SK15/lnx_make utils/lnx_make hw_api/lnx sw_daemons/lnx_make cmdman_console sw_api/lnx_make modules/typetools/lnx_make ";
$config = "configure.ac";
$base_config = "AC_CONFIG_FILES([Makefile\n\taloeSKs/aloeSK15/skel_SK15/lnx_make/Makefile\n\tutils/lnx_make/Makefile\n\thw_api/lnx/Makefile\n\tsw_daemons/lnx_make/Makefile\n\tsw_api/lnx_make/Makefile\n\tcmdman_console/Makefile\n\tmodules/typetools/lnx_make/Makefile\n\t";
$base_config_cnt = 7;

$modules_dir = "modules/";
$make_append = "lnx_make/";

@modules = <modules/*>;

$i=0;

print "Generating " . $makefile . " and " . $config . " files. For the following modules:\n";
foreach $module (@modules) {
	if ($module =~ m/typetools/) {
            $j=$i;
	}
	$i++;
	print $module . "\n";
}

splice(@modules,$j,1);

open(M,$makefile);
@lines = <M>;
close(M);

$output="";

$c=0;
foreach $line (@lines) {
	if ($line =~ m/^SUBDIRS/) {
		$output .= "SUBDIRS=" . $base_subdirs;
		foreach $module (@modules) {
			$output .= $module . "/" . $make_append . " ";
		}	
		$output .= "\n";
	} else {
		$output .= $line;
	}
}

open(M,">",$makefile);
print M $output;
close(M);

open(M,$config);
@lines = <M>;
close(M);

$output="";
$c=0;
foreach $line (@lines) {
	if ($line =~ m/^AC_CONFIG_FILES/ || ($c>0)) {
		if ($c==0) {
			$output .= $base_config;
			foreach $module (@modules) {
				$output .= "\t" . $module . "/" . $make_append . "Makefile\n";
			}		
			$output .= "\t])\n";
		}
		$c++;
		if ($line =~ m/\]\)/) {
			$c=0;
		}
	} else {
		$output .= $line;
	}
}
open(M,">",$config);
print M $output;
close(M);

print "Done.\n";




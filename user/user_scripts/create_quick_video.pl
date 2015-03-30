#!/usr/bin/perl

if (@ARGV < 7) {
	print "Usage: perl $0 textrenderoutputfile videofile arenasize-x arenasize-y mintime inttime maxtime \[highlightid1 ...\] \n";

	print "Create a simple 2D video from the output of the text renderer\n";
	print "Parameters:\n";
	print " - textrenderoutputfile: the name of the file with the output of the text renderer\n";
	print " - videofile: the name of the produced video file (should be .avi)\n";
	print " - arenasize-x: length of the arena x axis in meters\n";
	print " - arenasize-y: length of the arena y axis in meters\n";
	print " - mintime: time step at which video should start (the program assumes each timestep lasts 0.1 s\n";
	print " - inttime: number of time steps between each frame of the video\n";
	print " - maxtime: time step at which video should end\n";
	print " - highlightid1, ...: possibility to give some robot ids that should be highlighted (shown larger)\n";

	print "Remarks: the video currently only shows footbots.\n";

	exit;
}

$renderfile = $ARGV[0];
$videofile = $ARGV[1];
$arenax = $ARGV[2] * 100;
$arenay = $ARGV[3] * 100;
$mintime = $ARGV[4];
$inttime = $ARGV[5];
$maxtime = $ARGV[6];
for ($i = 5;$i < @ARGV;$i++) {
	$highlightids[$i-5] = $ARGV[$i];
}

$tempdir = "tempjpgs";
system "mkdir $tempdir";
open(IN,"$renderfile");
$line = <IN>;
$t = $mintime;
while ($t < 0) {
	$t += $inttime;
}
print "Time $t\n";
$command = "";
$highlightcommand = "";
while ($line = <IN>) {
	$line =~ /^([0-9]+)/;
	$tline = $1;
	if ($tline > $t) {
		$imagefile = sprintf("frame%06d.jpg", $t);
		$command = "convert -size $arenax\lx$arenay xc:white -fill blue" . $command . " $tempdir\/$imagefile";
		system "$command";
		if ($t < $maxtime) {
			$command = "";
			$t += $inttime;
			print "Time $t\n";
		} else {
			last;
		}
	}
	if ($tline == $t) {
		$line =~ /([a-zA-Z]+)\s+([a-zA-Z0-9._-]+)\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)/;
		$command .= AddEntity($1, $2, $3, $4, $5, $6, $7, $8);
	}
}
close IN;
$fps = 10 / $inttime;
system "mencoder mf:\/\/$tempdir\/*.jpg -mf w=$arenax:h=$arenay:fps=$fps -o $videofile -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=3000";
system "rm -r $tempdir";

sub AddEntity {
	my $type = $_[0];
	my $id = $_[1];
	my $x = $_[2] * 100;
	my $y = $_[3] * 100;
	my $z = $_[4] * 100;
	my $a = $_[5];
	my $b = $_[6];
	my $g = $_[7];
	my $returnstring = "";
	if ($type eq "Footbot") {
		$xlow = $x - 5;
		for (my $i = 0;$i < @highlightids;$i++) {
			if ($highlightids[$i] eq $id) {
				$xlow = $x - 10;
				last;
			}
		}
		$returnstring = " -draw \"circle $x,$y $xlow,$y\"";
	} elsif ($type eq "Block") {
	} else {
	}
	return $returnstring;
}


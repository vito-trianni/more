#!/usr/bin/perl

use XML::Twig;
#use strict;

if (@ARGV < 4) {
	print "Usage: perl $0 inputxmlfile outputxmlfile value xmltreenode [xmltreenode ...]\n";

	print "Arguments:\n";
	print "- inputxmlfile: The input xml file\n";
	print "- outputxmlfile: The output xml file\n";
	print "- value: The new value for the xml node\n";
	print "- xmltreenode: give the nodes of the xml tree down to the one you want to change the value of (but don't include the root node \"ahss-config\")\n";
	print "  (instead of a node name, you can also give an id value)\n";

	print "This program changes the value of a node in an xml file.\n";
	print "Example Usage:\n";
	print "\"perl $0 sample_experiment.xml sample_experiment.xml 200 framework random_seed\"\n";
	print "This changes the random_seed parameter in the sample_experiment.xml file\n";
	print "\nIMPORTANT: you should have the perl modules XML Parser and XML Twig installed\n\n";

	exit;
}

$inputfile = $ARGV[0];
$outputfile = $ARGV[1];
$value = $ARGV[2];
for ($i = 0, $j = 3; $j < @ARGV; $i++, $j++) {
	$xmltreenode[$i] = $ARGV[$j];
}

$twig = new XML::Twig;
$twig->parsefile( "$inputfile");

$found = true;
$node = $twig->root;
change_value($node,0);

$twig->set_pretty_print('indented');
open(OUT,">$outputfile");
$twig->print(\*OUT);
close OUT;

sub change_value { # takes 2 parameters: node to change and current index
	my $curr_node = $_[0];
	my $curr_index = $_[1];

	# Try to get the nodes with the right name or id:
    my @nodes = $curr_node->children($xmltreenode[$curr_index]);
	if (@nodes == 0) {
	    @nodes = $curr_node->children("*\[\@id=\"$xmltreenode[$curr_index]\"\]");
		if (@nodes == 0) {
			return;
		}
	}

	# Go through the nodes and change them:
	for (my $i = 0; $i < @nodes; $i++) {
		if ($curr_index == @xmltreenode - 1) {
			$nodes[$i]->set_text($value);
		}
		else {
			change_value($nodes[$i],$curr_index+1);
		}
	}
}

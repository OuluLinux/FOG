#
#	Test script for FOG
#
#	Prerequisite:	TestFog	is executable to test
#		so on Unix do something like
#				ln -s sun4o_g/fog TestFog
#		and on NT create TestFog.bat containing
#				Debug/Fog $*
#
#	Invoke as:
#
#	perl test.pl <test-directory>*
#		requiring the TestFog prerequisite or as e.g.
#	perl test.pl executable="\"Debug/fog.exe\"" <test-directory>*
#		to use a specific executable for the tests
#
#	where each test directory comprises
#		'test-directory'/'test-directory'.fog
#		optional include files
#		'test-directory'/references/'reference result files'
#
#	running test creates
#		'test-directory'/results/'actual result files'
#
#	Thus to run the full set (wildcard only works on Unix)
#		perl test.pl tests/*
#	Thus to run some tests (NT or Unix)
#		perl test.pl tests/ansi_c tests/lexical
#
#	Warning: The stream synchronisation on NT is abysmal so diagnostics resulting
#	from invocation of FOG may appear extraordinarily early wrt other activities.
#
#print "executable = $executable\n";
eval "\$$1=\$2" while $ARGV[0] =~ /^(\w+)=(.*)/ && shift;		# Nutshell perl p256
#print "executable = $executable\n";
$executable = "\"./TestFog\"" unless $executable ne "";
print "executable = $executable\n";
$ENV{'TESTNAME'} = "willink";									# Used by getenv.fog
$failures = 0;
$passes = 0;
$tests = 0;
for (@ARGV)
{
	next if $_ eq '.';
	next if $_ eq '..';
	next unless -d $_;
	print "$_\n";
	$error_count = 0;
	$tests++;
	local($dir) = $_;
	mkdir($dir .'/references', ~umask) unless -e $dir .'/references';
	if (-e $dir .'/results')
	{
		opendir(DIR,$dir .'/results') || die "Can't open $dir/results";
		local($unlinks) = "$dir/results/" . join(" $dir/results/", grep(!/^\.\.?$/, readdir(DIR)));
		local(@unlinks) = split(/ /,$unlinks);
#		print "unlink @unlinks\n";
		unlink @unlinks;
		closedir(DIR);
	}
	else
	{
		mkdir($dir .'/results', ~umask) unless -e $dir .'/results';
	}
	opendir(DIR, $dir) || die "Can't open $dir";
	local(@commandline) = "$executable -q -zd -t4 -cd $dir/results -hd $dir/results $dir/" .
							join(' $dir/',grep(/\.fog$/, readdir(DIR))) .
					" -nc -nobanner -i$dir -global global -DMACRO=aMacro -log $dir/results/log.log";
	closedir(DIR);
	print "@commandline\n";
	system "@commandline\n";
	if (-e $dir . '/results/log.log')
	{
		opendir(DIR,$dir .'/results') || die "Can't open $dir/results";
		local(@resultnames) = grep(!/^\.\.?$/, readdir(DIR));
		closedir(DIR);
		for (@resultnames)
		{
#			print "result $_\n";
			if (!-e $dir .'/references/' . 	$_)
			{
				print "ERROR missing result need to cp $dir/results/$_ $dir/references/$_.\n";
				$error_count++;
			}
			else
			{
				local(@commandline) = "diff -b $dir/references/$_ $dir/results/$_ > $dir/results/$_.diff";
#				print "@commandline\n";
				$diff_status = (system "@commandline\n") / 256;
#				print "$diff_status\n";
				if ($diff_status)
				{
					print "ERROR need to verify and then cp $dir/results/$_ $dir/references/$_.\n";
					open(DIFFS, $dir . '/results/' . $_ . '.diff') || warn "Can't open $dir/results/$_.diff\n";
					while (<DIFFS>)
					{
						print;
					}
					$error_count++;
				}
#				print "diff $dir/results/$_ $dir/references/$_.\n";
#				open(RESULT, $dir . '/results/' . $_) || warn "Can't open $dir/results/$_.\n";
#				open(REFERENCE, $dir . '/references/' . $_) || warn "Can't open $dir/references/$_.\n";
#				$break = 0;
#				while (!eof(RESULT) && !eof(REFERENCE) && !$break)
#				{
#					@reswords = split(/\W*\s+\W*/, <RESULT>);
#					@refwords = split(/\W*\s+\W*/, <REFERENCE>);
#					if (join(" ", @refwords) ne join(" ", @reswords))
#					{
#						print "Files differ.\n";
#						print "    Reference: " , join(" ", @refwords), "\n";
#						print "    Result:    " , join(" ", @reswords), "\n";
#						$break++;
#					}
#				}
#				if (!eof(RESULT) || !eof(REFERENCE) || $break)
#				{
#					print "ERROR need to verify and then cp $dir/results/$_ $dir/references/$_.\n";
#					$error_count++;
#				}
#				close RESULT;
#				close REFERENCE;
			}
		}
	}
	else
	{
		print "ERROR fog failed to execute.\n";
		$error_count++;
	}
	if (-e $dir . '/references/log.log')
	{
		opendir(DIR,$dir .'/references') || die "Can't open $dir/references";
		local(@referencenames) = readdir(DIR);
		closedir(DIR);
		for (@referencenames)
		{
			next if $_ eq '.';
			next if $_ eq '..';
			next if $_ eq "Debug";						# Ignore bogus Debug directory
			next if $_ eq "Release";					# Ignore bogus Release directory
			next if $_ eq "vssver.scc";					# Ignore Visual SourceSafe control files
			if (!-e $dir .'/results/' . 	$_)
			{
				print "ERROR $dir/results/$_ not generated to correspond to $dir/references/$_.\n";
				$error_count++;
			}
		}
	}
	if ($error_count > 0)
	{
		print "FAILED $dir with $error_count errors ---------------------------------------------------.\n";
		$failures++;
	}
	else
	{
		print "PASSED $dir ---------------------------------------------------.\n";
		$passes++;
	}
}
if ($failures > 0)
{
	print "FAILED $failures out of $tests tests.\n";
}
if ($passes > 0)
{
	print "PASSED $passes out of $tests tests.\n";
}

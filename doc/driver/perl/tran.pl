#!/usr/bin/perl -w
#
# File: trans.pl
# �\��: ���ɮץi�H�� DOS �榡������(\r\n)�A�ഫ�� Linux �t
#       �ΨϥΪ�����r��(\n)
# �� wade@Fantasy.Space �Ҵ���
# �w����N�����P�ϥ�
# �p��������D�A���p���Gjjchen@alumni.ice.ntnu.edu.tw
#
use File::Find;
@ARGV = ('.') unless @ARGV;

find (\&mychange, @ARGV);

sub mychange {
  my ($file, $tmp);

  $file = $_;
  if (-d $file) {
    return;
  }
  $tmp = "tmp.";
  open (FILE, "< $file") || die "Cannot open file $file: $!";
  open (TMP, "> $tmp") || die "Cannot open file $tmp: $!";
  while (<FILE>) {
    # ���U�� \r �N�O�����Ҧb�C
    s/\r//;
    print TMP $_;
  }
  close (FILE);
  close (TMP);
  unlink ($file);
  rename ($tmp, $file) || warn "Cannot rename $tmp to $file: $!";
}


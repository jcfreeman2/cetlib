use strict;

package CetSkel::BasicPluginCommon;

use Exporter;

use vars qw(@ISA @EXPORT @EXPORT_OK);
use vars qw($bp_headers);
@ISA = qw(Exporter);
@EXPORT = qw($bp_headers);
@EXPORT_OK = qw(&pluginSuffix);

sub pluginSuffix {
  return "_plugin";
}

1;

### Local Variables:
### mode: cperl
### End:

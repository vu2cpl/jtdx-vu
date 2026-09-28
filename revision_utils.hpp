#ifndef REVISION_UTILS_HPP__
#define REVISION_UTILS_HPP__

#include <QString>

QString revision (QString const& scs_rev_string = QString {});
QString version (bool include_patch = true);  // upstream JTDX base version
QString jtdxvu_version ();                     // JTDX-VU release version
QString program_title (QString const& revision = QString {});

#endif

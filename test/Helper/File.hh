<?hh //strict

namespace HHPack\Migrate\Test\Helper\File;

use SplFileInfo;

// In the case of realpath(), it becomes consider type annotations
function absolutePath(string $path) : string
{
    $f = new SplFileInfo($path);
    return $f->getPathname();
}

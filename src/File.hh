<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\File;

use SplFileInfo;
use SplFileObject;

function readAll(string $path) : string
{
    $f = new SplFileObject($path);
    return $f->fread($f->getSize());
}

function readJsonFile(string $path) : array<string, mixed>
{
    $json = readAll($path);
    return json_decode($json, true);
}

// In the case of realpath(), it becomes consider type annotations
function absolutePath(string $path) : string
{
    $f = new SplFileInfo($path);
    return $f->getPathname();
}

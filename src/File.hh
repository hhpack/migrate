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

use HHPack\Migrate\{FileNotFoundException};

use SplFileInfo;
use SplFileObject;

function read_all(string $path): string {
  if (!file_exists($path)) {
    throw new FileNotFoundException("File $path is not found");
  }

  $f = new SplFileObject($path);
  return $f->fread($f->getSize());
}

function read_json_file(string $path): array<string, mixed> {
  $json = read_all($path);
  return json_decode($json, true);
}

// In the case of realpath(), it becomes consider type annotations
function absolute_path(string $path): string {
  $f = new SplFileInfo($path);
  return $f->getPathname();
}

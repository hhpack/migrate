<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

interface Logger {
  public function debug(string $message): void;
  public function info(string $message): void;
  public function warn(string $message): void;
  public function error(string $message): void;
}

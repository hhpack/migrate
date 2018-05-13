<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application;

use HHPack\Migrate\{Output, Console};
use RuntimeException;

final class Kernel {
  public function __construct(private Output $console = new Console()) {}

  public function run(Traversable<string> $argv): void {
    try {
      (new MigrateApplication($this->console))->run($argv);
    } catch (RuntimeException $e) {
      $this->terminateWithError($e);
    }
  }

  private function terminateWithError(RuntimeException $e): void {
    $this->console->writeError("Application error:\n");
    $this->console
      ->writeError(\sprintf("%d: %s\n", $e->getCode(), $e->getMessage()));
    exit(-1);
  }

}

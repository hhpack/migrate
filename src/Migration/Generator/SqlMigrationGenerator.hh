<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Migration\Generator;

use HHPack\Migrate\{File, Logger};
use HHPack\Migrate\Migration\{MigrationGenerator, GenerateVersion};
use DateTime;

final class SqlMigrationGenerator implements MigrationGenerator {

  public function __construct(
    private string $directory,
    private Logger $logger,
  ) {}

  public function generate(GenerateVersion $newVersion): void {
    $identity = $newVersion['version'].'_'.$newVersion['name'];

    $path = File\absolute_path($this->directory);
    $up = sprintf("%s/%s.up.sql", $path, $identity);
    $down = sprintf("%s/%s.down.sql", $path, $identity);

    touch($up);
    $this->logger->info(sprintf("File %s created\n", $up));

    touch($down);
    $this->logger->info(sprintf("File %s created\n", $down));
  }

}

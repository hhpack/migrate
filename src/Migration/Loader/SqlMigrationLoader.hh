<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Migration\Loader;

use HHPack\Migrate\{
  File,
  MigrationName,
  Migration,
  DirectoryNotFoundException
};
use HHPack\Migrate\Migration\{MigrationLoader};
use HHPack\Migrate\Migration\Type\{SqlMigration};

final class SqlMigrationLoader implements MigrationLoader {

  private string $directory;

  public function __construct(string $directory) {
    if (!file_exists($directory) || !is_dir($directory)) {
      throw new DirectoryNotFoundException("Directory $directory not found");
    }
    $this->directory = $directory;
  }

  public function loadUpgradeMigrations(): ImmVector<Migration> {
    $pattern = File\absolutePath($this->directory).'/*.up.sql';
    $files = $this->findFiles($pattern);

    $files = ImmSet::fromItems($files)->toValuesArray();
    asort(&$files);

    return
      ImmVector::fromItems($files)
        ->map(($file) ==> SqlMigration::fromFile($file));
  }

  public function loadDowngradeMigrations(
    Traversable<MigrationName> $migrations,
  ): ImmVector<Migration> {
    $appliedMigrations = ImmSet::fromItems($migrations);

    $pattern = File\absolutePath($this->directory).'/*.down.sql';
    $files = $this->findFiles($pattern);

    $files = ImmSet::fromItems($files)->toValuesArray();
    arsort(&$files);

    return
      ImmVector::fromItems($files)
        ->map(($file) ==> SqlMigration::fromFile($file))
        ->filter(
          ($migration) ==> $appliedMigrations->contains($migration->name()),
        );
  }

  private function findFiles(string $pattern): Iterator<string> {
    foreach (glob($pattern) as $file) {
      yield $file;
    }

    // An error may appear in the check type, do not use the constant.
    // GLOB_NOSORT = 4, GLOB_ONLYDIR = 8192
    $directories = glob(dirname($pattern).'/*', 4 | 8192);
    foreach ($directories as $directory) {
      $files = $this->findFiles($directory.'/'.basename($pattern));
      foreach ($files as $file) {
        yield $file;
      }
    }
  }

}

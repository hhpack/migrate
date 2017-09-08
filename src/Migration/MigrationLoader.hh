<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Migration;

use HHPack\Migrate\{Migration, MigrationName};

interface MigrationLoader {
  /**
   * Load the migration for the upgrade
   */
  public function loadUpgradeMigrations(): ImmVector<Migration>;

  /**
   * Load the migration for the downgrade
   */
  public function loadDowngradeMigrations(
    Traversable<MigrationName> $migrations,
  ): ImmVector<Migration>;
}

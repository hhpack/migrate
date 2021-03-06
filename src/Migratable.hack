/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

use HHPack\Migrate\Migration\MigrationResult;

interface Migratable {
  public function upgrade(): Awaitable<MigrationResult>;
  public function downgrade(): Awaitable<MigrationResult>;
  public function upgradeTo(MigrationName $name): Awaitable<MigrationResult>;
  public function downgradeTo(MigrationName $name): Awaitable<MigrationResult>;
}

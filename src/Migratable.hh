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

interface Migratable
{
    public function upgrade(?MigrationName $name): Awaitable<MigrationResult>;
    public function downgrade(MigrationName $name): Awaitable<MigrationResult>;
}

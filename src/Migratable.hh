<?hh //strict

/**
 * This file is part of hhpack\migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace hhpack\migrate;

interface Migratable
{
    public function upgrade(?MigrationName $name = null): Awaitable<MigrationResult>;
    public function downgrade(MigrationName $name): Awaitable<MigrationResult>;
}

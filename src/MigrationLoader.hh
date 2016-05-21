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

final class MigrationLoader implements MigrationLoadable
{

    public function __construct(
        private MigrationLoadable $loader
    )
    {
    }

    public function loadUpMigration(): ImmVector<Migration>
    {
        return $this->loader->loadUpMigration();
    }

    public function loadDownMigration(Traversable<string> $migrations): ImmVector<Migration>
    {
        return $this->loader->loadDownMigration($migrations);
    }

}

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

use hhpack\publisher\Message;

final class MigrationLoadedEvent implements Message
{

    public function __construct(
        private ImmVector<Migration> $migrations
    )
    {
    }

    public function queries(): ImmVector<string>
    {
        return $this->migrations->map(($migration) ==> $migration->query());
    }

    public function migrationCount(): int
    {
        return $this->migrations->count();
    }

}

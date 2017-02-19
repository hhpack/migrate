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

use HHPack\Publisher\Message;

final class MigrationStartEvent implements Message
{

    public function __construct(
        private string $query
    )
    {
    }

    public function query(): string
    {
        return $this->query;
    }

}

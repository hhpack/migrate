<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database\Query;

use HHPack\Migrate\Database\{ Query, QueryResult };
use AsyncMysqlConnection;

final class CreateMigrationsTableQuery implements Query
{

    const string TABLE_NAME = 'scheme_migrations';

    public async function execute(AsyncMysqlConnection $connection): Awaitable<QueryResult>
    {
        $result = await $connection->query(sprintf(
            'CREATE TABLE IF NOT EXISTS %s (`id` int(11) NOT NULL AUTO_INCREMENT, `name` varchar(255), `run_at` datetime NOT NULL, PRIMARY KEY (`id`))',
            $connection->escapeString(static::TABLE_NAME)
        ));
        $rows = $result->mapRowsTyped()
            ->map(($row) ==> $row->toImmMap())
            ->toImmVector();

        return new QueryResult('CREATE DATABASE %s', $rows, $result->startTime(), $result->endTime());
    }

}

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

final class SelectMigrationsQuery implements Query
{

    public function __construct(
        private string $tableName
    )
    {
    }

    public async function execute(AsyncMysqlConnection $connection): Awaitable<QueryResult>
    {
        $result = await $connection->queryf(
            'SELECT name FROM %T ORDER BY run_at DESC',
            $this->tableName
        );

        $rows = $result->mapRowsTyped()
            ->map(($row) ==> $row->toImmMap())
            ->toImmVector();

        return new QueryResult('CREATE DATABASE %s', $rows, $result->startTime(), $result->endTime());
    }

}

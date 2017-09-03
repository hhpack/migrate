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

final class CreateDatabaseQuery implements Query
{

    public function __construct(
        private string $dbName
    )
    {
    }

    public async function execute(AsyncMysqlConnection $connection): Awaitable<QueryResult>
    {
        $result = await $connection->queryf('CREATE DATABASE %T', $this->dbName);

        $rows = $result->mapRowsTyped()
            ->map(($row) ==> $row->toImmMap())
            ->toImmVector();

        return new QueryResult($rows, $result->startTime(), $result->endTime());
    }

}

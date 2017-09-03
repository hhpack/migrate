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

use HHPack\Migrate\{ MigrationName };
use HHPack\Migrate\Database\{ Query, QueryResult };
use AsyncMysqlConnection;

final class RemoveMigrationQuery implements Query
{

    public function __construct(
        private string $tableName,
        private MigrationName $name
    )
    {
    }

    public async function execute(AsyncMysqlConnection $connection): Awaitable<QueryResult>
    {
        $result = await $connection->queryf(
            'DELETE FROM %T WHERE %C %=s',
            $this->tableName,
            'name',
            $this->name
        );
        return QueryResult::fromAsyncResult($result);
    }

}

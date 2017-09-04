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

final class DatabaseAlreadyExistsQuery implements Query
{

    const string TABLE_NAME = 'SCHEMATA';
    const string COLUMN_NAME = 'SCHEMA_NAME';

    public function __construct(
        private string $name
    )
    {
    }

    public async function execute(AsyncMysqlConnection $connection): Awaitable<QueryResult>
    {
        $result = await $connection->queryf(
            'SELECT %C FROM %T WHERE %C %=s',
            static::COLUMN_NAME,
            static::TABLE_NAME,
            static::COLUMN_NAME,
            $this->name
        );

        return QueryResult::fromAsyncResult($result);
    }

}

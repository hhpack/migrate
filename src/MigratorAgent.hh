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

final class MigratorAgent implements QueryProxy
{

    public function __construct(
        private Connection $connection,
        private EventPublisher $publisher
    )
    {
    }

    public async function query(string $query): Awaitable<QueryResult>
    {
        await $this->publisher->migrationStart($query);
        $result = await $this->connection->query($query);
        await $this->publisher->migrationSuccess($result);

        return $result;
    }

}

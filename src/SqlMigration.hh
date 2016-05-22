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

final class SqlMigration implements Migration
{

    private ImmSet<string> $queries;

    public function __construct(
        private string $name,
        Traversable<string> $queries
    )
    {
        $this->queries = ImmSet::fromItems($queries);
    }

    public function name(): string
    {
        return $this->name;
    }

    public function version(): string
    {
        return preg_replace("/^(\d+)\-.+/", "$1", $this->name);
    }

    public function queries(): ImmSet<string>
    {
        return $this->queries;
    }

    public async function change(QueryProxy $proxy): Awaitable<ImmVector<QueryResult>>
    {
        $results = Vector {};

        foreach ($this->queries as $query) {
            $result = await $proxy->query($query);
            $results->add($result);
        }

        return $results->toImmVector();
    }

    public static function fromFile(string $file): this
    {
        if (!file_exists($file)) {
            throw new FileNotFoundException("$file is not found");
        }

        $absolutePath = realpath($file);

        $name = preg_replace("/\.(down|up)\..+$/", "", basename($absolutePath));
        $sql = file_get_contents($absolutePath);

        $queries = explode(';', $sql);

        return new SqlMigration($name, $queries);
    }

}

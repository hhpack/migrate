<?hh //strict

namespace hhpack\migrate;

final class SqlMigration implements Migration
{

    public function __construct(
        private string $query
    )
    {
    }

    public async function change(QueryProxy $proxy): Awaitable<QueryResult>
    {
        return await $proxy->query($this->query);
    }

    public static function fromFile(string $file): this
    {
        $sql = file_get_contents($file);
        return new SqlMigration($sql);
    }

}

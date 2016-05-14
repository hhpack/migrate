<?hh //strict

namespace hhpack\migrate;

final class SqlMigration implements Migration
{

    public function __construct(
        private string $version,
        private string $query
    )
    {
    }

    public function version(): string
    {
        return $this->version;
    }

    public async function change(QueryProxy $proxy): Awaitable<QueryResult>
    {
        return await $proxy->query($this->query);
    }

    public static function fromFile(string $file): this
    {
        $version = preg_replace("/^(\d+)\_.+/", "$1", basename($file));
        $sql = file_get_contents($file);

        return new SqlMigration($version, $sql);
    }

}

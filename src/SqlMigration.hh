<?hh //strict

namespace hhpack\migrate;

final class SqlMigration implements Migration
{

    public function __construct(
        private string $name,
        private string $query
    )
    {
    }

    public function name(): string
    {
        return $this->name;
    }

    public function version(): string
    {
        return preg_replace("/^(\d+)\-.+/", "$1", $this->name);
    }

    public function query(): string
    {
        return $this->query;
    }

    public async function change(QueryProxy $proxy): Awaitable<QueryResult>
    {
        return await $proxy->query($this->query);
    }

    public static function fromFile(string $file): this
    {
        if (!file_exists($file)) {
            throw new FileNotFoundException("$file is not found");
        }

        $absolutePath = realpath($file);

        $name = preg_replace("/\.(down|up)\..+$/", "", basename($absolutePath));
        $sql = file_get_contents($absolutePath);

        return new SqlMigration($name, $sql);
    }

}

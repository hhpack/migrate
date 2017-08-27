<?hh //strict

namespace HHPack\Migrate\Application\Test;

use HHPack\Migrate\{ File, DataSourceName, Connection };
use HHPack\Migrate\Application\{ Context, UpCommand, DownCommand };
use HHPack\Migrate\Test\Mock\{ MigrateContext };
use HHPack\Migrate\Test\Helper\{ Db };
use HackPack\HackUnit\Contract\Assert;


final class DownCommandTest
{

    public function __construct(
        private Connection $conn,
        private Context $upContext,
        private Context $downContext
    )
    {
    }

    <<Setup('test')>>
    public function setUpTest() : void
    {
        \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS scheme_migrations") );
        \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS users") );
        \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS posts") );
    }

    <<SuiteProvider('Default')>>
    public static function noArguments() : this
    {
        $path = File\absolutePath(__DIR__ . '/../sql/migrations/');

        $conn = Db\connect();
        $upContext = new MigrateContext($path, [], $conn);
        $downContext = new MigrateContext($path, ['20150824010439-create-users'], $conn);

        return new static($conn, $upContext, $downContext);
    }

    <<Test('Default')>>
    public function migrateToLatestSchema(Assert $assert): void
    {
        $command = new UpCommand();
        $command->run($this->upContext);

        $command = new DownCommand();
        $command->run($this->downContext);

        $result = \HH\Asio\join($this->conn->query('show tables'));
        $rows = $result->rows();

        // scheme_migrations
        $assert->int($rows->count())->eq(1);
    }
}

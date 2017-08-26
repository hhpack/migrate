<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\Test\Helper\{ Db };
use HHPack\Migrate\{ File, Migrator, Connection, SqlMigrationLoader };
use HHPack\Migrate\Logger\{ ColoredLogger };
use HackPack\HackUnit\Contract\Assert;

final class MigratorTest
{
    public function __construct(
        private Connection $conn,
        private Migrator $migrator
    )
    {
    }

    <<SuiteProvider('Db')>>
    public static function create() : this
    {
        $conn = Db\connect();
        $path = File\absolutePath(__DIR__ . '/sql/migrations');

        $loader = new SqlMigrationLoader($path);
        $migrator = new Migrator($loader, $conn, new ColoredLogger());

        return new static($conn, $migrator);
    }

    <<Setup('test')>>
    public function setUpTest() : void
    {
        \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS scheme_migrations") );
        \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS users") );
        \HH\Asio\join( $this->conn->query("DROP TABLE IF EXISTS posts") );
    }

    <<Test('Db')>>
    public function upgrade(Assert $assert): void
    {
        $result = \HH\Asio\join( $this->migrator->upgrade() );
        $assert->int($result->resultCount())->eq(2);
    }

    <<Test('Db')>>
    public function downgrade(Assert $assert): void
    {
        \HH\Asio\join( $this->migrator->upgrade() );
        $result = \HH\Asio\join( $this->migrator->downgrade('20150824010439-create-users') );

        $assert->int($result->resultCount())->eq(2);

        $results = $result->at('20150825102100-create-posts');
        $assert->string($results->at(0)->query())->is("DROP TABLE IF EXISTS `posts`");
    }
}

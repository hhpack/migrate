<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\Test\Helper\{ Db };
use HHPack\Migrate\{ File, Migrator };
use HHPack\Migrate\Migration\{  MigrationNotFoundException };
use HHPack\Migrate\Migration\Loader\{ SqlMigrationLoader };
use HHPack\Migrate\Database\{ Connection };
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
        \HH\Asio\join( $this->conn->rawQuery("DROP TABLE IF EXISTS scheme_migrations") );
        \HH\Asio\join( $this->conn->rawQuery("DROP TABLE IF EXISTS users") );
        \HH\Asio\join( $this->conn->rawQuery("DROP TABLE IF EXISTS posts") );
    }

    <<Test('Db')>>
    public function upgrade(Assert $assert): void
    {
        $result = \HH\Asio\join( $this->migrator->upgrade() );
        $assert->int($result->resultCount())->eq(2);
    }

    <<Test('Db')>>
    public function downgradeByLastName(Assert $assert): void
    {
        \HH\Asio\join( $this->migrator->upgrade() );
        $result = \HH\Asio\join( $this->migrator->downgrade('20150824010439-create-users') );

        $assert->int($result->resultCount())->eq(2);

        $posts = $result->at('20150825102100-create-posts');
        $assert->bool($posts->containsKey(0))->is(true);

        $users = $result->at('20150824010439-create-users');
        $assert->bool($users->containsKey(0))->is(true);
    }

    <<Test('Db')>>
    public function downgradeByFirstName(Assert $assert): void
    {
        \HH\Asio\join( $this->migrator->upgrade() );
        $result = \HH\Asio\join( $this->migrator->downgrade('20150825102100-create-posts') );

        $assert->int($result->resultCount())->eq(1);

        $posts = $result->at('20150825102100-create-posts');
        $assert->bool($posts->containsKey(0))->is(true);

        $assert->bool($result->containsKey('20150824010439-create-users'))->is(false);
    }

    <<Test('Db')>>
    public function downgradeAll(Assert $assert): void
    {
        \HH\Asio\join( $this->migrator->upgrade() );
        $result = \HH\Asio\join( $this->migrator->downgrade() );

        $assert->int($result->resultCount())->eq(2);

        $posts = $result->at('20150825102100-create-posts');
        $assert->bool($posts->containsKey(0))->is(true);

        $users = $result->at('20150824010439-create-users');
        $assert->bool($users->containsKey(0))->is(true);
    }

    <<Test('Db')>>
    public function downgradeMigrationNotFound(Assert $assert): void
    {
        \HH\Asio\join( $this->migrator->upgrade() );

        $assert->whenCalled(() ==> {
            \HH\Asio\join( $this->migrator->downgrade('20150825102999-not-found') );
        })->willThrowClass(MigrationNotFoundException::class);
    }

}

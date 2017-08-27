<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\Test\Helper\{ Db };
use HHPack\Migrate\{ MigrationManager };
use HHPack\Migrate\Database\{ Connection };
use HackPack\HackUnit\Contract\Assert;


final class MigrationManagerTest
{
    public function __construct(
        private Connection $conn,
        private MigrationManager $manager
    )
    {
    }

    <<SuiteProvider('Db')>>
    public static function create() : this
    {
        $conn = Db\connect();
        $manager = new MigrationManager($conn);

        return new static($conn, $manager);
    }

    <<Setup('test')>>
    public function setUpTest() : void
    {
        \HH\Asio\join( $this->conn->query('DROP TABLE IF EXISTS scheme_migrations') );
    }

    <<Test('Db')>>
    public function setup(Assert $assert): void
    {
        \HH\Asio\join( $this->manager->setUp() );
        $result = \HH\Asio\join( $this->conn->query('SHOW CREATE TABLE scheme_migrations') );

        $rows = $result->rows();
        $assert->int($rows->count())->eq(1);
    }

    <<Test('Db')>>
    public function loadMigrations(Assert $assert): void
    {
        \HH\Asio\join( $this->manager->setUp() );
        \HH\Asio\join( $this->conn->query("INSERT INTO scheme_migrations (name, run_at) VALUES ('20150824010439.up.sql', CURRENT_TIMESTAMP)") );
        \HH\Asio\join( $this->conn->query("INSERT INTO scheme_migrations (name, run_at) VALUES ('20150825102100.up.sql', CURRENT_TIMESTAMP)") );

        $result = \HH\Asio\join( $this->manager->loadMigrations() );

        \HH\Asio\join( $this->conn->query("DELETE FROM scheme_migrations") );

        $assert->int($result->count())->eq(2);
    }
}

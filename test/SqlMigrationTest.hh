<?hh //strict

namespace HHPack\Migrate\Test;

use HHPack\Migrate\test\helper;
use HHPack\Migrate\SqlMigration;
use HHPack\Migrate\EventPublisher;
use HHPack\Migrate\MySqlConnection;
use HHPack\Migrate\MigratorAgent;
use HHPack\Migrate\FileNotFoundException;
use HackPack\HackUnit\Contract\Assert;

final class SqlMigrationTest
{

    public function __construct(
        private MigratorAgent $agent
    )
    {
    }

    <<SuiteProvider('Db')>>
    public static function create() : this
    {
        $conn = helper\connect();
        $agent = new MigratorAgent($conn, new EventPublisher());

        return new static($agent);
    }

    <<Test('Db')>>
    public function migrationFile(Assert $assert): void
    {
        $sql = SqlMigration::fromFile(realpath(__DIR__ . '/sql/20150824010439-show-tables.up.sql'));

        $assert->string($sql->version())->is('20150824010439');
        $assert->string($sql->name())->is('20150824010439-show-tables');

        $results = \HH\Asio\join( $sql->change($this->agent) );
        $assert->int($results->count())->eq(1);
    }

    <<Test('Db')>>
    public function fileNotFoundMigrationFile(Assert $assert): void
    {
        $assert->whenCalled(() ==> {
            $sql = SqlMigration::fromFile(__DIR__ . '/sql/not_found.up.sql');
        })->willThrowClass(FileNotFoundException::class);
    }
}

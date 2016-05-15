<?hh //strict

namespace hhpack\migrate;

final class SqlMigrationLoader implements MigrationLoadable
{

    public function __construct(
        private string $directory
    )
    {
    }

    public function loadUpMigration(): ImmVector<Migration>
    {
        $pattern = realpath($this->directory) . '/*.up.sql';
        $files = $this->findFiles($pattern);

        $files = ImmSet::fromItems($files)->toValuesArray();
        asort($files);

        return ImmVector::fromItems($files)
            ->map(($file) ==> SqlMigration::fromFile($file));
    }

    public function loadDownMigration(Traversable<string> $migrations): ImmVector<Migration>
    {
        $appliedMigrations = ImmSet::fromItems($migrations);

        $pattern = realpath($this->directory) . '/*.down.sql';
        $files = $this->findFiles($pattern);

        $files = ImmSet::fromItems($files)->toValuesArray();
        arsort($files);

        return ImmVector::fromItems( $files )
            ->map(($file) ==> SqlMigration::fromFile($file))
            ->filter(($migration) ==> $appliedMigrations->contains( $migration->name() ) );
    }

    private function findFiles(string $pattern) : Iterator<string>
    {
        foreach (glob($pattern) as $file) {
            yield $file;
        }

        // An error may appear in the check type, do not use the constant.
        // GLOB_NOSORT = 4, GLOB_ONLYDIR = 8192
        $directories = glob(dirname($pattern) . '/*', 4 | 8192);
        foreach ($directories as $directory) {
            $files = $this->findFiles($directory . '/' . basename($pattern));
            foreach ($files as $file) {
                yield $file;
            }
        }
    }

}

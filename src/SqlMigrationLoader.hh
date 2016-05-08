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
        $pattern = realpath($this->directory) . '/*_up.sql';
        $files = $this->findFiles($pattern);

        return ImmVector::fromItems($files)
            ->map(($file) ==> SqlMigration::fromFile($file));
    }

    public function loadDownMigration(): ImmVector<Migration>
    {
        $pattern = realpath($this->directory) . '/*_down.sql';
        $files = $this->findFiles($pattern);

        return ImmVector::fromItems( $files )
            ->map(($file) ==> SqlMigration::fromFile($file));
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

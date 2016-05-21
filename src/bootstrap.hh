<?hh //strict

namespace hhpack\migrate;

/**
 * name of migration
 * 
 * @example
 * 20150824010439-create-users
 * 20150825102100-create-posts
 */
newtype MigrationName as string = string;
